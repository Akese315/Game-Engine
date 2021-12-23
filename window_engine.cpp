#include "window_engine.h"




window_engine::window_engine(VkInstance instance, Device* device, uint32_t width, uint32_t height)
{	
	this->_swapchainExtent.width = width;
	this->_swapchainExtent.height = height;
	this->deviceObj = device;
	this->VulkanInstance = instance;
	if (init() > 0)
	{
		return;
	}
}

int window_engine::init()
{
	
	if (!glfwInit())
	{

		Log::error("Bibliothèque non initialisé !");
		glfwTerminate();
		return 10;
	}
	glfwWindowHint(GLFW_CLIENT_API,GLFW_NO_API);
	_window = glfwCreateWindow(_swapchainExtent.width, _swapchainExtent.height, "Game Engine", NULL, NULL);
	

	if (!_window)
	{
		
		Log::error("Fenêtre non initialisé !");
		glfwTerminate();
		return 10;
	}
	glfwMakeContextCurrent(_window);
	glfwSetWindowUserPointer(_window, this);
	glfwSetFramebufferSizeCallback(_window, framebufferResizeCallback);
	glfwSetWindowPos(_window, 710, 290);
	CreateSurface();
	swapChainInit();
	initVertexBuffer();
	PipelineInit();	
	CommandBufferInit();
	createSyncObjects();
	
	
}

void window_engine::loop()
{
	while (!glfwWindowShouldClose(_window))
	{		
		glfwPollEvents();
		drawFrame();	
	}
	vkDeviceWaitIdle(deviceObj->getDevice());

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		vkDestroySemaphore(deviceObj->getDevice(), _renderFinishedSemaphore[i], nullptr);
		vkDestroySemaphore(deviceObj->getDevice(), _imageAvailableSemaphore[i], nullptr);
		vkDestroyFence(deviceObj->getDevice(), _inFlightFences[i], nullptr);
	}	
	CommandBufferDeInit();
	deInitVertexBuffer();
	PipelineDeInit();	
	swapChainDeInit();
	vkDestroySurfaceKHR(VulkanInstance, _surface, nullptr);
	glfwTerminate();
	
}

void window_engine::drawFrame()
{
	VkResult result = vkWaitForFences(deviceObj->getDevice(), 1, &_inFlightFences[_currentFrame], VK_TRUE, UINT64_MAX);
	uint32_t imageIndex;
	result = vkAcquireNextImageKHR(deviceObj->getDevice(), swapchainObj->getSwapChain(), UINT64_MAX, _imageAvailableSemaphore[_currentFrame], VK_NULL_HANDLE, &imageIndex);
	// this function get the next image
	if (result == VK_ERROR_OUT_OF_DATE_KHR) {
		Log::error("trop de temps", result);
		WindowResized();
		return;
	}
	else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
		Log::error("failed to acquire swap chain image!",result);
	}

	if (_imagesInFlight[imageIndex] != VK_NULL_HANDLE) {
		vkWaitForFences(deviceObj->getDevice(), 1, &_imagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);
	}
	_imagesInFlight[imageIndex] = _inFlightFences[_currentFrame];

	VertexObj->updateUniformBuffer(imageIndex);

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore waitSemaphores[]		= { _imageAvailableSemaphore[_currentFrame] };
	VkPipelineStageFlags waitStages[]	= { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	submitInfo.waitSemaphoreCount		= 1;
	submitInfo.pWaitSemaphores			= waitSemaphores;
	submitInfo.pWaitDstStageMask		= waitStages;

	vector<VkCommandBuffer> commandList = commandBufferObj->getCommandBuffer();
	VkCommandBuffer currentCommand = commandList[imageIndex];

	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = (const VkCommandBuffer*)&currentCommand;

	VkSemaphore signalSemaphores[] = { _renderFinishedSemaphore[_currentFrame] };
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	vkResetFences(deviceObj->getDevice(), 1, &_inFlightFences[_currentFrame]);

	result = vkQueueSubmit(deviceObj->getQueue(), 1, &submitInfo, _inFlightFences[_currentFrame]);

	if (result != VK_SUCCESS) {
		Log::error("failed to submit draw command buffer!",result);
	}

	VkPresentInfoKHR presentInfo{};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;

	VkSwapchainKHR swapChains[] = { swapchainObj->getSwapChain() };
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;

	presentInfo.pImageIndices = &imageIndex;

	result = vkQueuePresentKHR(deviceObj->getQueue(), &presentInfo);

	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || framebufferResized) {
		Log::error("window resized", result);
		framebufferResized = false;		
		WindowResized();		
	}
	else if (result != VK_SUCCESS) {
		Log::error("failed to present swap chain image!",result);
	}

	//ALL FAILED TO PRESENT THE IMAGE

	_currentFrame = (_currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void window_engine::createSyncObjects()
{
	
	_imageAvailableSemaphore.resize(MAX_FRAMES_IN_FLIGHT);
	_renderFinishedSemaphore.resize(MAX_FRAMES_IN_FLIGHT);
	_inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
	_imagesInFlight.resize((swapchainObj->getSwapchainImage()).size(), VK_NULL_HANDLE);

	VkSemaphoreCreateInfo semaphoreInfo{};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkFenceCreateInfo fenceInfo{};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;


	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		VkResult error=	vkCreateSemaphore(deviceObj->getDevice(), &semaphoreInfo, nullptr, &_imageAvailableSemaphore[i]);
		if (error != VK_SUCCESS)
		{
			Log::error("Available semaphore failed to create", error);
		}
		error = vkCreateSemaphore(deviceObj->getDevice(), &semaphoreInfo, nullptr, &_renderFinishedSemaphore[i]);
		if (error != VK_SUCCESS)
		{
			Log::error("Finished Semaphore failed to create", error);
		}
		error = vkCreateFence(deviceObj->getDevice(), &fenceInfo, nullptr,	&_inFlightFences[i]);
		if (error != VK_SUCCESS)
		{
			Log::error("Fence failed to create", error);
		}
	}
}

 void window_engine::framebufferResizeCallback(GLFWwindow* window, int width,int height)
{
	 window_engine* app = reinterpret_cast<window_engine*>(glfwGetWindowUserPointer(window));
	 app->framebufferResized = true;
	 
}


void window_engine::CreateSurface()
{
	VkWin32SurfaceCreateInfoKHR createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	createInfo.hwnd = glfwGetWin32Window(_window);
	createInfo.hinstance = GetModuleHandle(nullptr);

	VkResult error = vkCreateWin32SurfaceKHR(VulkanInstance, &createInfo, nullptr, &_surface);
	if ( error != VK_SUCCESS) {

		Log::error("failed to create window surface!",error);
		return;
	}
	Log::success("Surface created !");
}

VkExtent2D window_engine::getCurrentWindowSize()
{	
	VkSurfaceCapabilitiesKHR capabilitiesInfo{};
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(*deviceObj->getPhysicalDevice(), _surface ,&capabilitiesInfo);

	return capabilitiesInfo.currentExtent;
}


void window_engine::WindowResized()
{
	vkDeviceWaitIdle(deviceObj->getDevice());
	
	commandBufferObj->cleanUp();
	VertexObj		->cleanUp();
	renderer		->cleanUp();
	swapchainObj	->cleanUp();
	swapchainObj	->recreateSwapChain();
	_swapchainExtent = swapchainObj->getCurrentWindowSize();
	renderer		->recreatePipeline();
	VertexObj		->recreateVertexObj();
	commandBufferObj->recreateCommandObj();
	_imagesInFlight.resize((swapchainObj->getSwapchainImage()).size(), VK_NULL_HANDLE);
}

void window_engine::startConsole()
{
}

void window_engine::swapChainInit()
{
	this->swapchainObj	= new SwapChain(this->deviceObj, &_surface, &_swapchainExtent);
	cout << swapchainObj->getSwapChain() << endl;
}

void window_engine::swapChainDeInit()
{
	delete swapchainObj;
}

void window_engine::PipelineInit()
{
	renderer = new vulkan_render(this->deviceObj, &_swapchainExtent, swapchainObj, VertexObj);
	
	
}
void window_engine::PipelineDeInit()
{
	delete renderer;
}

void window_engine::initVertexBuffer()
{
	VertexObj = new Vertex(this->deviceObj, swapchainObj);
}

void window_engine::deInitVertexBuffer()
{
	delete VertexObj;
}

void window_engine::CommandBufferInit()
{
	commandBufferObj = new CommandBuffer(this->deviceObj, this->swapchainObj,this->renderer, &this->_swapchainExtent, this->VertexObj);
}

void window_engine::CommandBufferDeInit()
{
	delete commandBufferObj;
}
