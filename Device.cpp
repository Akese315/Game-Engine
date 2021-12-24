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
	getLayersProperties();
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


	VkDeviceCreateInfo device_create_info{};  
	device_create_info.sType				= VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	device_create_info.queueCreateInfoCount = 1;
	device_create_info.pQueueCreateInfos = &device_queue_info;
	device_create_info.enabledExtensionCount = _extensionCount;
	device_create_info.ppEnabledExtensionNames = _extNames.data();
	
	

	error = vkCreateDevice(_gpu, &device_create_info, nullptr, &_device);
	if (VK_SUCCESS != error)
	{
		Log::error("erreur lors de la création du device", error);
		exit(-1);
	}
	
	Log::success("The device has been created");
	
}

void Device::getQueueFamily()
{
	
	vkGetDeviceQueue(_device, _graphics_family_index, 0,&_graphicsQueue);

}

void Device::getLayersProperties()
{
	uint32_t count = 0;
	vkEnumerateDeviceLayerProperties(_gpu, &count, NULL);
	vector<VkLayerProperties> layerPropsList(count);
	VkResult result = vkEnumerateDeviceLayerProperties(_gpu, &count, layerPropsList.data());
	for (VkLayerProperties property : layerPropsList)
	{
		cout << property.layerName << " description : " << property.description << endl;
	}
}

VkQueue Device::getQueue()
{

	return _graphicsQueue;

}

void Device::ShowGPUprops()
{
	cout << _gpu_properties.deviceName << endl;
	cout << _gpu_properties.deviceID << endl;
	
}

uint32_t Device::getGraphics_family_index()
{
	return _graphics_family_index;
}