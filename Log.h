#pragma once
#include <string>
#include <iostream>
#include <vulkan/include/vulkan.h>

using namespace std;
	



class Log
{	

public:

	static void error(string message, VkResult error = VK_ERROR_UNKNOWN);
	static void warning(const string& message);
	template <typename T>
	
	static void message(T  message)
	{	
		
	
		
	
		string c1 = "\x1B[33m";
		string c2 = "\033[0m\t\t";
		string formatedMessage = c1 + "   MESSAGE :" + c2;
		printf(formatedMessage.c_str());
		std::cout << message << endl;
	}
	static void success(string message);
	
private:
	static string ConvertErrorToString(VkResult error);
};