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
	};

	VKAPI_ATTR VkBool32 VKAPI_CALL DebugUtilsMessenger_DebugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* callbackData,
		void* userData);

}