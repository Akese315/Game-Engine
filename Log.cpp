#include "Log.h";


void Log::error(string message, VkResult error)
{
	cout <<"ERROR : "<< message << "Error : " << ConvertErrorToString(error)<<endl;
	

}
void  Log::warning(string message, int importance)
{
	cout <<"WARNING : "<< message << importance << endl;

	
}
void  Log::message(string message)
{
	cout <<"MESSAGE : " << message << endl;
	
}

void  Log::success(string message)
{
	cout << "SUCCESS : "<<message << endl;

}

string Log::ConvertErrorToString(VkResult error)
{
    string exception = "NULL";
	switch (error)
	{
    case VK_SUCCESS :
        exception = "VK_SUCCESS";
        break;
    case VK_NOT_READY :
        exception = "VK_NOT_READY";
        break;
    case VK_TIMEOUT :
        exception = "VK_TIMEOUT";
        break;
    case VK_EVENT_SET :
        exception = "VK_EVENT_SET";
        break;
    case VK_EVENT_RESET :
        exception = "VK_EVENT_RESET";
        break;
    case VK_INCOMPLETE :
        exception = "VK_INCOMPLETE";
        break;
    case  VK_ERROR_OUT_OF_HOST_MEMORY :
        exception = "VK_ERROR_OUT_OF_HOST_MEMORY";
        break;
    case VK_ERROR_OUT_OF_DEVICE_MEMORY :
        exception = "VK_ERROR_OUT_OF_DEVICE_MEMORY";
        break;
    case VK_ERROR_INITIALIZATION_FAILED :
        exception = "VK_ERROR_INITIALIZATION_FAILED";
        break;
    case VK_ERROR_DEVICE_LOST :
        exception = "VK_ERROR_DEVICE_LOST";
        break;
    case VK_ERROR_MEMORY_MAP_FAILED :
        exception = "VK_ERROR_MEMORY_MAP_FAILED";
        break;
    case VK_ERROR_LAYER_NOT_PRESENT :
        exception = "VK_ERROR_LAYER_NOT_PRESENT";
        break;
    case VK_ERROR_EXTENSION_NOT_PRESENT :
        exception = "VK_ERROR_EXTENSION_NOT_PRESENT";
        break;
    case VK_ERROR_FEATURE_NOT_PRESENT :
        exception = "VK_ERROR_FEATURE_NOT_PRESENT";
        break;
    case VK_ERROR_INCOMPATIBLE_DRIVER :
        exception = "VK_ERROR_INCOMPATIBLE_DRIVER";
        break;
    case VK_ERROR_TOO_MANY_OBJECTS :
        exception = "VK_ERROR_TOO_MANY_OBJECTS";
        break;
    case VK_ERROR_FORMAT_NOT_SUPPORTED:
        exception = "VK_ERROR_FORMAT_NOT_SUPPORTED";
        break;
    case VK_ERROR_FRAGMENTED_POOL:
        exception = "VK_ERROR_FRAGMENTED_POOL";
        break;
    case VK_ERROR_UNKNOWN:
        exception = "VK_ERROR_UNKNOWN";
        break;
    case VK_ERROR_OUT_OF_POOL_MEMORY:
        exception = "VK_ERROR_OUT_OF_POOL_MEMORY / VK_ERROR_OUT_OF_POOL_MEMORY_KHR";
        break;
    case VK_ERROR_INVALID_EXTERNAL_HANDLE:
        exception = "VK_ERROR_INVALID_EXTERNAL_HANDLE / VK_ERROR_INVALID_EXTERNAL_HANDLE_KHR ";
        break;
    case VK_ERROR_FRAGMENTATION:
        exception = "VK_ERROR_FRAGMENTATION / VK_ERROR_FRAGMENTATION_EXT";
        break;
    case VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS:
        exception = string{ "VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS / VK_ERROR_INVALID_DEVICE_ADDRESS_EXT\n" 
            "VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS_KHR / " };
            
        break;
    case VK_ERROR_SURFACE_LOST_KHR:
        exception = "VK_ERROR_SURFACE_LOST_KHR";
        break;
    case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:
        exception = "VK_ERROR_NATIVE_WINDOW_IN_USE_KHR";
        break;
    case VK_SUBOPTIMAL_KHR:
        exception = "VK_SUBOPTIMAL_KHR";
        break;
    case VK_ERROR_OUT_OF_DATE_KHR:
        exception = "VK_ERROR_OUT_OF_DATE_KHR";
        break;
    case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR:
        exception = "VK_ERROR_INCOMPATIBLE_DISPLAY_KHR";
        break;
    case VK_ERROR_VALIDATION_FAILED_EXT:
        exception = "VK_ERROR_VALIDATION_FAILED_EXT";
        break;
    case VK_ERROR_INVALID_SHADER_NV:
        exception = "VK_ERROR_INVALID_SHADER_NV";
        break;
    case VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT:
        exception = "VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT";
        break;
    case VK_ERROR_NOT_PERMITTED_EXT:
        exception = "VK_ERROR_NOT_PERMITTED_EXT";
        break;
    case VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT:
        exception = "VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT";
        break;
    case VK_THREAD_IDLE_KHR:
        exception = "VK_THREAD_IDLE_KHR";
        break;
    case VK_THREAD_DONE_KHR:
        exception = "VK_THREAD_DONE_KHR";
        break;
    case VK_OPERATION_DEFERRED_KHR:
        exception = "VK_OPERATION_DEFERRED_KHR";
        break;
    case VK_PIPELINE_COMPILE_REQUIRED_EXT:
        exception = "VK_PIPELINE_COMPILE_REQUIRED_EXT / VK_ERROR_PIPELINE_COMPILE_REQUIRED_EXT";
        break;
    case VK_RESULT_MAX_ENUM:
        exception = "VK_RESULT_MAX_ENUM";
      
	}

    return exception;
}