#pragma once

#include "Zinet/VulkanRenderer/ZtVulkanRendererConfig.hpp"
#include "Zinet/VulkanRenderer/ZtVulkanObject.hpp"

#include "Zinet/Core/ZtLogger.hpp"

#include <vulkan/vulkan.h>

namespace zt::vulkan_renderer
{
	class Instance;

	class ZINET_VULKAN_RENDERER_API DebugUtilsMessenger : public VulkanObject<VkDebugUtilsMessengerEXT>
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("VRDebugUtilsMessenger");

	public:

		DebugUtilsMessenger() noexcept = default;
		DebugUtilsMessenger(const DebugUtilsMessenger& other) noexcept = delete;
		DebugUtilsMessenger(DebugUtilsMessenger&& other) noexcept = default;
		~DebugUtilsMessenger() noexcept = default;

		DebugUtilsMessenger& operator = (const DebugUtilsMessenger& other) noexcept = delete;
		DebugUtilsMessenger& operator = (DebugUtilsMessenger&& other) noexcept = default;

		bool create(Instance& instance) noexcept;

		void destroy(Instance& instance) noexcept;

		static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
			VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
			VkDebugUtilsMessageTypeFlagsEXT messageType,
			const VkDebugUtilsMessengerCallbackDataEXT* callbackData,
			void* userData);

	};

	inline VkBool32 DebugUtilsMessenger::DebugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* callbackData,
		void* userData)
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