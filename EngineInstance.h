#pragma once
#include <vulkan/include/vulkan.h>
#include "Log.h"
#include <vector>
class EngineInstance
{
public:
	EngineInstance();
	~EngineInstance();
	VkInstance getInstance();
	
	
private:
	VkInstance _instance;
	vector<const char*>	_instance_extensions;

	void checkExtensions();
};

