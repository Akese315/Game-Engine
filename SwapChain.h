#pragma once
#include <vulkan/include/vulkan.h>
#include <vector>
#include "Device.h"
#include "Log.h"
class SwapChain{	

	private:
		//foreigner object
		Device*						deviceObj = NULL;
		VkSurfaceKHR*				surface = NULL;

		//personnal class object

		struct SwapChainSupportDetails {
			VkSurfaceCapabilitiesKHR capabilities;
			vector<VkSurfaceFormatKHR> formats;
			vector<VkPresentModeKHR> presentModes;
		};


		VkSwapchainKHR				_swapchain = NULL;
		vector<VkImageView>			_swapchainImageView;
		VkExtent2D*					extent;
		VkSurfaceFormatKHR			_SwapchainFormat;
		uint32_t					_imageCount = 0;
		vector<VkImage>				_swapChainImages;
		SwapChainSupportDetails		_details;
		VkPresentModeKHR			_PresentMode;

		

		
		VkPresentModeKHR			getAdequatePresentMode();		
		void						initSwapchain();
		void						InitSwapImage();
		void						querySwapchainSupport();
		VkSurfaceFormatKHR			getAdequateFormat();
		


	public:
									SwapChain(Device* device, VkSurfaceKHR* surface, VkExtent2D* extent);
									~SwapChain();
		VkSwapchainKHR				getSwapChain();
		VkFormat					getFormat();
		vector<VkImageView>			getSwapChainImageView();
		vector<VkImage>				getSwapchainImage();
		VkExtent2D					getCurrentWindowSize();
		void						recreateSwapChain();
		void						cleanUp();
		VkImageView					createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);

		
		

	
};

