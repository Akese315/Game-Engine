#include "Device.h";

Device::Device(VkPhysicalDevice gpu)
{
	_gpu = gpu;	
	getPorperties();
	init_QueueFamily();
	getQueueFamily();
	
}

Device::~Device()
{

	vkDestroyDevice(_device,nullptr);
	_device = nullptr;
}

void Device::getPorperties()
{
	vkGetPhysicalDeviceProperties(_gpu, &_gpu_properties);
	ShowGPUprops();
}

VkDevice Device::getDevice()
{
	return _device;
}

VkPhysicalDevice* Device::getPhysicalDevice()
{
	return &_gpu;
}

uint32_t Device::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(_gpu, &memProperties);
	for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) 
	{
		if((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) ==properties) {
			return i;			
		}		
	}
}


void Device::init_QueueFamily()
{	
	{
		uint32_t family_count = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(_gpu, &family_count, nullptr);
		vector<VkQueueFamilyProperties> family_property_list(family_count);
		vkGetPhysicalDeviceQueueFamilyProperties(_gpu, &family_count, family_property_list.data());


		bool found = false;
		for (uint32_t i = 0; i < family_count; i++)
		{
			if (family_property_list[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				found = true;
				_graphics_family_index = i;
			}
		}

		if (!found)
		{
			Log::error("Queue family supporting graphics not found");
			exit(-1);
		}
	}
	uint32_t _extensionCount = 0;
	vkEnumerateDeviceExtensionProperties(_gpu, nullptr, &_extensionCount, nullptr);
	vector<VkExtensionProperties> _extension(_extensionCount);
	VkResult error = vkEnumerateDeviceExtensionProperties(_gpu, nullptr, &_extensionCount, _extension.data());
	for (uint32_t i = 0; i < _extension.size(); i++)
	{
		_extNames.push_back(_extension[i].extensionName);
	}
	if (_extNames.size() == 0)
	{
		Log::error("erreur lors de la recherche d'extansion ", error);
		exit(-1);
	}

	float queue_priorities[]{ 1.0f };
	VkDeviceQueueCreateInfo device_queue_info{};
	device_queue_info.sType				= VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	device_queue_info.queueFamilyIndex	= _graphics_family_index;
	device_queue_info.queueCount		= 1;
	device_queue_info.pQueuePriorities	= queue_priorities;

	VkPhysicalDeviceFeatures deviceFeatures{};
	deviceFeatures.samplerAnisotropy = VK_TRUE;
	VkDeviceCreateInfo device_create_info{};  
	device_create_info.sType				= VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	device_create_info.queueCreateInfoCount = 1;
	device_create_info.pQueueCreateInfos = &device_queue_info;
	device_create_info.enabledExtensionCount = _extensionCount;
	device_create_info.ppEnabledExtensionNames = _extNames.data();
	device_create_info.pEnabledFeatures;
	
	

	error = vkCreateDevice(_gpu, &device_create_info, nullptr, &_device);
	if (VK_SUCCESS != error)
	{
		Log::error("erreur lors de la création du device", error);
		exit(-1);
	}
	
	Log::success("The device has been created");
	
}

void Device::isDeviceSuitable(VkPhysicalDevice device) 
{
	/*
	QueueFamilyIndices indices = findQueueFamilies(device);

	bool extensionsSupported = checkDeviceExtensionSupport(device);

	bool swapChainAdequate = false;
	if (extensionsSupported) {
		SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device);
		swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
	}

	VkPhysicalDeviceFeatures supportedFeatures;
	vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

	return indices.isComplete() && extensionsSupported && swapChainAdequate && supportedFeatures.samplerAnisotropy;
	*/}

void Device::getQueueFamily()
{
	
	vkGetDeviceQueue(_device, _graphics_family_index, 0,&_graphicsQueue);

}

VkQueue Device::getQueue()
{

	return _graphicsQueue;

}

void Device::ShowGPUprops()
{
	
	Log::message<char *>(_gpu_properties.deviceName);
	switch (_gpu_properties.deviceType)
	{
	case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
		Log::message<string>("GPU TYPE : VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU");
		break;
	case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
		Log::message<string>("GPU TYPE : VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU");
		break;
	case VK_PHYSICAL_DEVICE_TYPE_CPU:
		Log::message<string>("GPU TYPE : VK_PHYSICAL_DEVICE_TYPE_CPU");
		break;
	case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
		Log::message<string>("GPU TYPE : VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU");
		break;
	default:
		Log::message<string>("GPU TYPE : Other type of device");
		break;
	}
	
}

VkFormat Device::findSupportedFormat(const std::vector<VkFormat>&
	candidates, VkImageTiling tiling, VkFormatFeatureFlags features)
{
	for (VkFormat format : candidates) {
		VkFormatProperties props;
		vkGetPhysicalDeviceFormatProperties(_gpu, format, &props);
		
		if (tiling == VK_IMAGE_TILING_LINEAR &&
			(props.linearTilingFeatures & features) == features) {
			return format;
		}
		else if (tiling == VK_IMAGE_TILING_OPTIMAL &&
			(props.optimalTilingFeatures & features) == features) {
			return format;
		}
	}
	Log::error("No supported format detected :(");
}

VkFormat Device::findDepthFormat() {
	return findSupportedFormat(	{VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT},		
		 VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
		);}
bool Device::hasStencilComponent(VkFormat format) {
	return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;	
}

uint32_t Device::getGraphics_family_index()
{
	return _graphics_family_index;
}

