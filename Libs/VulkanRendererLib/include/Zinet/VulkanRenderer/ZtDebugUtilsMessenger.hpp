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

		DebugUtilsMessenger(HandleType newObjectHandle)
			: VulkanObject(newObjectHandle) {}

		DebugUtilsMessenger() ZINET_API_POST = delete;
		DebugUtilsMessenger(const DebugUtilsMessenger& other) ZINET_API_POST = delete;
		DebugUtilsMessenger(DebugUtilsMessenger&& other) ZINET_API_POST = default;
		~DebugUtilsMessenger() ZINET_API_POST = default;

		DebugUtilsMessenger& operator = (const DebugUtilsMessenger& other) ZINET_API_POST = delete;
		DebugUtilsMessenger& operator = (DebugUtilsMessenger&& other) ZINET_API_POST = default;

		bool create(const Instance& instance) ZINET_API_POST;

		void destroy(const Instance& instance) ZINET_API_POST;
	};

	VKAPI_ATTR VkBool32 VKAPI_CALL DebugUtilsMessenger_DebugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* callbackData,
		void* userData);

}