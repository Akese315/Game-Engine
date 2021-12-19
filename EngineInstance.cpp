#include "EngineInstance.h"

EngineInstance::EngineInstance()
{
	
	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
	vector<VkExtensionProperties> extention_props(extensionCount);
	VkResult error =vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extention_props.data());
	for (uint32_t i = 0; i < extention_props.size(); i++)
	{
		_instance_extensions.push_back(extention_props[i].extensionName);
	}
	if (_instance_extensions.size() == 0)
	{
		Log::error("erreur lors de la recherche d'extansion ", error);
		exit(-1);
	}
	
	//check extensions  

	VkApplicationInfo application_info{};
	application_info.sType				= VK_STRUCTURE_TYPE_APPLICATION_INFO;
	application_info.apiVersion			= VK_API_VERSION_1_2;
	application_info.pApplicationName	= "My Engine";

	//Application info stucture

	VkInstanceCreateInfo instance_create_info{};
	instance_create_info.sType				= VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instance_create_info.pApplicationInfo	= &application_info;
	instance_create_info.enabledExtensionCount = _instance_extensions.size();
	instance_create_info.ppEnabledExtensionNames = _instance_extensions.data();

	//Instance info structure

	error = vkCreateInstance(&instance_create_info, nullptr, &_instance); 
	
	//create the instances

	if (VK_SUCCESS != error)
	{
		Log::error("vk instance failed !", error);
		exit(-1);
	}
	Log::success("Instance created !");

	//check if error during instance creation
}

EngineInstance::~EngineInstance()
{
	vkDestroyInstance(_instance, nullptr);
	_instance = nullptr;	
}

VkInstance  EngineInstance::getInstance()
{
	return _instance;
}

void EngineInstance::checkExtensions()
{
	
}