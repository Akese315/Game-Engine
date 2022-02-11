#pragma once
#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#include <vulkan/include/vulkan.h>
#include <gl/GL.h>
#include "mainClass.h"
#include "Log.h"
#include "Device.h"
#include "SwapChain.h"
#include "vulkan_render.h"
#include "CommandBuffer.h"
#include "Vertex.h"
#include "WindowEvent.h"


class window_engine
{

private:
	const int MAX_FRAMES_IN_FLIGHT = 2;

	GLFWwindow*					_window = nullptr;
	vector<VkSemaphore>			_imageAvailableSemaphore;
	vector<VkSemaphore>			_renderFinishedSemaphore;
	vector<VkFence>				_inFlightFences;
	vector<VkFence>				_imagesInFlight;
	VkSurfaceKHR				_surface;
	VkExtent2D					_swapchainExtent;
	size_t						_currentFrame = 0;
	bool						framebufferResized = false;


	VkInstance					VulkanInstance;
	Device*						deviceObj;
	SwapChain*					swapchainObj;
	vulkan_render*				renderer;
	CommandBuffer* 				commandBufferObj;
	Vertex*						VertexObj;
	mainClass*					mainClassObj;
	WindowEvent*				WindowEventObj;
		
	


	

	void createWindowEvent();
	void destroyWindowEvent();
	void swapChainDeInit();
	void swapChainInit();
	void CreateSurface();
	void PipelineInit();
	void PipelineDeInit();
	void createSyncObjects();
	void initVertexBuffer();
	void deInitVertexBuffer();
	void drawFrame();
	void CommandBufferInit();
	void CommandBufferDeInit();
	void WindowResized();
	void createMainClass();
	void destroyMainClass();
	void startConsole();
	VkExtent2D getCurrentWindowSize();
	static void framebufferResizeCallback(GLFWwindow* window, int width, int height);

public:
	window_engine(VkInstance instance, Device *device, uint32_t width, uint32_t height);
	GLFWwindow* getWindow();
	void loop();
	int init();
	






};

