#include "Vulkan_Obj.h"


Vulkan_Obj::Vulkan_Obj()
{	
	init();
	
}

void Vulkan_Obj::init()
{
	engineInstance = new EngineInstance();
	instance = engineInstance->getInstance();
	getAllDevice();
	selectMainDevice();
	windowObj = new window_engine(instance, mainDevice,1000,800);
	readFile("Triangle.txt");
	windowObj->loop();
}

Vulkan_Obj::~Vulkan_Obj()
{
	DeleteAllDevice();
	delete windowObj;
	delete engineInstance;
	
}

void Vulkan_Obj::getAllDevice()
{	
	vkEnumeratePhysicalDevices(instance, &GPU_NUMBER, nullptr);
	vector<VkPhysicalDevice> Device_List(GPU_NUMBER);
	vkEnumeratePhysicalDevices(instance, &GPU_NUMBER, Device_List.data());
	for (size_t i = 0; i < GPU_NUMBER; i++)
	{	
		Device* device = new Device(Device_List[i]);
		DeviceObject_List.push_back(device);
	}
};

void Vulkan_Obj::selectMainDevice()
{
	vector<uint64_t> vramSize;
	VkPhysicalDeviceMemoryProperties memoryProps{};
	for (size_t i = 0; i < DeviceObject_List.size(); i++)
	{
		vkGetPhysicalDeviceMemoryProperties(*DeviceObject_List[i]->getPhysicalDevice(), &memoryProps);
		VkDeviceSize size = (memoryProps.memoryHeaps)->size;
		vramSize.push_back((uint32_t)size);
	}
	uint32_t bestgpu = 0;
	for (size_t i = 0; i < vramSize.size(); i++)
	{
		if (i == 0)
		{
			continue;
		}
		else
		{
			if (vramSize[bestgpu] < vramSize[i])
			{
				bestgpu = (uint32_t)i;
				continue;
			}
			if (vramSize[bestgpu] == vramSize[i])
			{
				continue;
			}
		}
	}
	mainDevice = DeviceObject_List[bestgpu];
	uint32_t hey = 0;
}



void Vulkan_Obj::DeleteAllDevice()
{
	for (size_t i = 0; i < DeviceObject_List.size(); i++)
	{
		delete DeviceObject_List.at(i);
	}
}

static std::vector<char> readFile(const std::string& filename) {
	 std::ifstream file(filename, std::ios::ate | std::ios::binary);
	
		 if (!file.is_open()) {
		 throw std::runtime_error(std::string{ "échec de l'ouverture du fichier "} + filename + "!");
			}
		  size_t fileSize = (size_t)file.tellg();
		  std::vector<char> buffer(fileSize);
		  file.seekg(0);
		  file.read(buffer.data(), fileSize);
		  file.close();
		  
		return buffer;
	}

void Vulkan_Obj::readFile(const std::string& filename) {
	std::ifstream file(filename);

	if (!file.is_open()) {
		Log::error(string{ "failed to read the shader file : " } + filename);
	}
	string line = "";
	while (getline(file, line))
	{
		cout << line<<endl;
	}
	file.close();
	return;
}