#pragma once

#include "Zinet/VulkanRenderer/ZtVulkanRendererConfig.hpp"
#include "Zinet/VulkanRenderer/ZtVulkanObject.hpp"

#include "Zinet/Core/ZtLogger.hpp"

#include <vulkan/vulkan.h>

namespace zt::vulkan_renderer
{
	class Instance;

	class  DebugUtilsMessenger : public VulkanObject<VkDebugUtilsMessengerEXT>
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("VRDebugUtilsMessenger");

	public:

		DebugUtilsMessenger(VulcanType newHandle)
			: VulkanObject(newHandle) {}

		DebugUtilsMessenger() noexcept = delete;
		DebugUtilsMessenger(const DebugUtilsMessenger& other) noexcept = delete;
		DebugUtilsMessenger(DebugUtilsMessenger&& other) noexcept = default;
		~DebugUtilsMessenger() noexcept = default;

		DebugUtilsMessenger& operator = (const DebugUtilsMessenger& other) noexcept = delete;
		DebugUtilsMessenger& operator = (DebugUtilsMessenger&& other) noexcept = default;

		bool create(const Instance& instance);

		void destroy(const Instance& instance) noexcept;
	};

	VKAPI_ATTR VkBool32 VKAPI_CALL DebugUtilsMessenger_DebugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* callbackData,
		void* userData);

}