#include "SwapChain.h"

SwapChain::SwapChain(Device* device, VkSurfaceKHR* surface, VkExtent2D* extent)
{
	this->deviceObj	= device;
	this->surface	= surface;
	this->extent	= extent;
	initSwapchain();
	InitSwapImage();
}




void SwapChain::initSwapchain()
{
	
	querySwapchainSupport();
	_SwapchainFormat = getAdequateFormat();
	_PresentMode = getAdequatePresentMode();
	VkSwapchainCreateInfoKHR swapchainInfo{};
	swapchainInfo.sType					= VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	swapchainInfo.surface				= *surface;
	swapchainInfo.minImageCount			= _details.capabilities.minImageCount+1;
	swapchainInfo.presentMode			= _PresentMode;
	swapchainInfo.clipped				= VK_TRUE;
	swapchainInfo.imageColorSpace		= _SwapchainFormat.colorSpace;
	swapchainInfo.imageExtent			= _details.capabilities.currentExtent;
	swapchainInfo.imageFormat			= _SwapchainFormat.format;
	swapchainInfo.preTransform			= VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
	swapchainInfo.imageSharingMode		= VK_SHARING_MODE_EXCLUSIVE;
	swapchainInfo.imageArrayLayers		= 1;
	swapchainInfo.imageUsage			= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	swapchainInfo.compositeAlpha		= VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	swapchainInfo.queueFamilyIndexCount = 0;
	swapchainInfo.pQueueFamilyIndices	= nullptr;
	
	if (_swapchain != NULL)
	{		
		vkDestroySwapchainKHR(deviceObj->getDevice(), _swapchain, nullptr);
		Log::message("swapchain détruite");		
	}	
	VkResult err = vkCreateSwapchainKHR(deviceObj->getDevice(), &swapchainInfo, nullptr, &_swapchain);
	if (err != VK_SUCCESS)
	{
		Log::error("Failed to create SwapchainKHR", err);
	}
	Log::success("Swapchain created");
}

void SwapChain::InitSwapImage()
{
	vkGetSwapchainImagesKHR(deviceObj->getDevice(), _swapchain, &_imageCount, nullptr);
	_swapChainImages.resize(_imageCount);
	vkGetSwapchainImagesKHR(deviceObj->getDevice(), _swapchain, &_imageCount, _swapChainImages.data());
	_swapchainImageView.resize(_swapChainImages.size());

	for (uint32_t i = 0; i < _swapChainImages.size(); i++)
	{
		_swapchainImageView[i] = createImageView(_swapChainImages[i], _SwapchainFormat.format, VK_IMAGE_ASPECT_COLOR_BIT);
		Log::success("SwapchainImage created ");
	}
}
VkImageView SwapChain::createImageView(VkImage& image, VkFormat format, VkImageAspectFlags aspectFlags)
{
	VkImageViewCreateInfo imageView_info{};
	imageView_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	imageView_info.image = image;
	imageView_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
	imageView_info.format = format;
	imageView_info.subresourceRange.aspectMask = aspectFlags;
	imageView_info.subresourceRange.baseMipLevel = 0;
	imageView_info.subresourceRange.levelCount = 1;
	imageView_info.subresourceRange.baseArrayLayer = 0;
	imageView_info.subresourceRange.layerCount = 1;

	VkImageView imageView;
	VkResult result = vkCreateImageView(deviceObj->getDevice(), &imageView_info, nullptr, &imageView);
	if (result != VK_SUCCESS)
	{
		Log::error("Failed to create imageView", result);
		return NULL;
	}
	return imageView;
}

void SwapChain::recreateSwapChain()
{	
	initSwapchain();
	InitSwapImage();
}

SwapChain::~SwapChain()
{	
	cleanUp();
	vkDestroySwapchainKHR(deviceObj->getDevice(), _swapchain, nullptr);
}

void SwapChain::cleanUp()
{
	
	for (VkImageView view : _swapchainImageView)
	{
		vkDestroyImageView(deviceObj->getDevice(), view, nullptr);
	}
}



VkPresentModeKHR SwapChain::getAdequatePresentMode()
{
	VkPresentModeKHR present_mode = VK_PRESENT_MODE_FIFO_KHR;
	for (VkPresentModeKHR mode : _details.presentModes)
	{
		if (mode == VK_PRESENT_MODE_MAILBOX_KHR)return mode;

	}
	return present_mode;
}

VkSurfaceFormatKHR SwapChain::getAdequateFormat()
{
	for (const auto& availableFormat : _details.formats) {
		 if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
			availableFormat.colorSpace ==	VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
			 return availableFormat;			
		}		
	}
	return _details.formats[0];


}

void SwapChain::querySwapchainSupport()
{
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(*deviceObj->getPhysicalDevice(), *surface,	&_details.capabilities);//récupère les capabilities
	if (_details.capabilities.currentExtent.height != UINT32_MAX && _details.capabilities.currentExtent.width != UINT32_MAX)
	{
		Log::message("Taille de la surface :");
		
		cout << "			hauteur"<<_details.capabilities.currentExtent.height << endl;
		cout << "			hauteur"<<_details.capabilities.currentExtent.width << endl;
	}
	else
	{
		Log::error("failed to get surface capabilities");
	}
	uint32_t formatCount = 0;
	vkGetPhysicalDeviceSurfaceFormatsKHR(*deviceObj->getPhysicalDevice(), *surface, &formatCount, nullptr);//récupère les formats
	_details.formats.resize(formatCount);
	vkGetPhysicalDeviceSurfaceFormatsKHR(*deviceObj->getPhysicalDevice(), *surface, &formatCount, _details.formats.data()); // récupère les formats
	if (formatCount == 0)
	{
		Log::message("No format found...");
	}
	uint32_t present_mode_count = 0;
	vkGetPhysicalDeviceSurfacePresentModesKHR(*deviceObj->getPhysicalDevice(), *surface, &present_mode_count, nullptr);
	if (present_mode_count != 0)
	{
		_details.presentModes.resize(present_mode_count);
		vkGetPhysicalDeviceSurfacePresentModesKHR(*deviceObj->getPhysicalDevice(), *surface, &present_mode_count, _details.presentModes.data());
	}
	else
	{
		Log::message("No present modes found...");
	}
}

VkFormat SwapChain::getFormat()
{
	return _SwapchainFormat.format;
}

VkExtent2D SwapChain::getCurrentWindowSize()
{
	return _details.capabilities.currentExtent;
}

vector<VkImageView> SwapChain::getSwapChainImageView() {
	return _swapchainImageView;
}
VkSwapchainKHR SwapChain::getSwapChain()
{
	return _swapchain;
}
vector<VkImage> SwapChain::getSwapchainImage()
{
	return _swapChainImages;
}
