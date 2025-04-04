#include "Zinet/VulkanRenderer/ZtDebugUtilsMessenger.hpp"
#include "Zinet/VulkanRenderer/ZtInstance.hpp"

namespace zt::vulkan_renderer
{
	bool DebugUtilsMessenger::create(Instance& instance)
	{
		VkDebugUtilsMessengerCreateInfoEXT createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_DEVICE_ADDRESS_BINDING_BIT_EXT;
		createInfo.pfnUserCallback = DebugCallback;
		createInfo.pUserData = nullptr;

		auto createFunc = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance.get(), "vkCreateDebugUtilsMessengerEXT");
		if (createFunc != nullptr)
		{
			VkResult result = std::invoke(createFunc, instance.get(), &createInfo, nullptr, &debugUtilsMessenger);
			
			if (result != VK_SUCCESS)
				Logger->error("Couldn't create DebugUtilsMessenger");

			return result == VK_SUCCESS;
		}
		else
		{
			Logger->error("Couldn't get vkCreateDebugUtilsMessengerEXT from vkGetInstanceProcAddr");
		}

		return false;
	}

	DebugUtilsMessenger::~DebugUtilsMessenger() noexcept
	{
		if (isValid())
			Logger->error("Object should be manualy invalidated");
	}

	void DebugUtilsMessenger::destroy(Instance& instance) noexcept
	{
		if (debugUtilsMessenger)
		{
			auto destroyFunc = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance.get(), "vkDestroyDebugUtilsMessengerEXT");
			if (destroyFunc != nullptr)
			{
				std::invoke(destroyFunc, instance.get(), debugUtilsMessenger, nullptr);
			}

			debugUtilsMessenger = nullptr;
		}
	}
}