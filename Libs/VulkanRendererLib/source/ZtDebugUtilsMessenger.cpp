#include "Zinet/VulkanRenderer/ZtDebugUtilsMessenger.hpp"
#include "Zinet/VulkanRenderer/ZtInstance.hpp"

namespace
{
	auto Logger = zt::core::ConsoleLogger::Create("zt::vulkan_renderer::DebugUtilsMessenger");
}

namespace zt::vulkan_renderer
{
	bool DebugUtilsMessenger::create(const Instance& instance)
	{
		if (isValid())
			return false;

		if (!instance.getEnableValidationLayers())
		{
			Logger->error("Instance has disabled validation layers");
			return false;
		}

		const VkDebugUtilsMessengerCreateInfoEXT createInfo
		{
			.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
			.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
			.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
			.pfnUserCallback = DebugUtilsMessenger_DebugCallback,
			.pUserData = nullptr
		};

		auto createFunc = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance.get(), "vkCreateDebugUtilsMessengerEXT");
		if (createFunc != nullptr)
		{
			VkResult result = std::invoke(createFunc, instance.get(), &createInfo, nullptr, &objectHandle);
			
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

	void DebugUtilsMessenger::destroy(const Instance& instance) noexcept
	{
		if (isValid())
		{
			auto destroyFunc = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance.get(), "vkDestroyDebugUtilsMessengerEXT");
			if (destroyFunc != nullptr)
			{
				std::invoke(destroyFunc, instance.get(), objectHandle, nullptr);
			}

			objectHandle = nullptr;
		}
	}

	VkBool32 DebugUtilsMessenger_DebugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		[[maybe_unused]] VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* callbackData,
		[[maybe_unused]] void* userData)
	{
		switch (messageSeverity)
		{
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
			Logger->trace(callbackData->pMessage);
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
			Logger->info(callbackData->pMessage);
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
			Logger->warn(callbackData->pMessage);
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
			Logger->error(callbackData->pMessage);
			break;
		default:
			break;
		}

		return VK_FALSE;
	}
}