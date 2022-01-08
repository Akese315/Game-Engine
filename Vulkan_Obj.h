#pragma once
#include "Log.h"
#include <vector>
#include "EngineInstance.h"
#include "Device.h"
#include "window_engine.h"
#include <iostream>
#include <fstream>
#include <string>

class Vulkan_Obj
{
public:
	Vulkan_Obj();
	~Vulkan_Obj();
	void getAllDevice();
	void DeleteAllDevice();
	
private:
	void selectMainDevice();
	void init();
	void readFile(const std::string& filename);

	EngineInstance * engineInstance = nullptr;
	window_engine* windowObj = nullptr;
	VkInstance instance;
	Device *mainDevice;
	Device* device = nullptr;
	vector<Device*> DeviceObject_List;
	uint32_t GPU_NUMBER = 0;
};

