#pragma once

#include "Zinet/VulkanRenderer/ZtVulkanRendererConfig.hpp"

#include "Zinet/Core/ZtLogger.hpp"

#include <vulkan/vulkan.h>

namespace zt::vulkan_renderer
{
	class Instance;

	class ZINET_VULKAN_RENDERER_API DebugUtilsMessenger
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("VrDebugUtilsMessenger");

	public:

		DebugUtilsMessenger() noexcept = default;
		DebugUtilsMessenger(const DebugUtilsMessenger& other) = delete;
		DebugUtilsMessenger(DebugUtilsMessenger&& other) noexcept = default;
		~DebugUtilsMessenger() noexcept;

		DebugUtilsMessenger& operator = (const DebugUtilsMessenger& other) = delete;
		DebugUtilsMessenger& operator = (DebugUtilsMessenger&& other) noexcept = default;

		bool create(Instance& instance);

		bool isValid() const noexcept { return debugUtilsMessenger; }

		void destroy(Instance& instance) noexcept;

		const auto get() const noexcept { return debugUtilsMessenger; }
		auto get() noexcept { return debugUtilsMessenger; }

		static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
			VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
			VkDebugUtilsMessageTypeFlagsEXT messageType,
			const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
			void* pUserData) {

			Logger->error(pCallbackData->pMessage);

			return VK_FALSE;
		}

	protected:

		VkDebugUtilsMessengerEXT debugUtilsMessenger = nullptr;

	};

}