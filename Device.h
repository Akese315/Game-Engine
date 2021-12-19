#pragma once
#include <vulkan/include/vulkan.h>
#include "Log.h"
#include <vector>

class Device
{

public:
	Device(VkPhysicalDevice gpu);
	~Device();
	VkPhysicalDevice * getPhysicalDevice();
	VkDevice getDevice();
	uint32_t getGraphics_family_index();
	VkQueue getQueue();
	uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
	

private:
	void ShowGPUprops();
	void getPorperties();
	void init_QueueFamily();
	void getQueueFamily();

	VkPhysicalDeviceProperties		_gpu_properties{};
	vector<const char*>				_extNames;
	VkDevice						_device;
	VkQueue							_graphicsQueue;
	VkPhysicalDevice				_gpu;
	uint32_t						_graphics_family_index = 0;
};

