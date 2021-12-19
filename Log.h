#pragma once
#include <string>
#include <iostream>
#include <vulkan/include/vulkan.h>

using namespace std;
class Log
{

public:
	static void error(string message, VkResult error = VK_ERROR_UNKNOWN);
	static void warning(string message, int importance);
	static void message(string message);
	static void success(string message);
private:
	static string ConvertErrorToString(VkResult error);
};