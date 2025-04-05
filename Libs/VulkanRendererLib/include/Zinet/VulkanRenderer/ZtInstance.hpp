#pragma once

#include "Zinet/VulkanRenderer/ZtVulkanRendererConfig.hpp"

#include "Zinet/Core/ZtLogger.hpp"

#include <vulkan/vulkan.h>

namespace zt::vulkan_renderer
{
	class ZINET_VULKAN_RENDERER_API Instance
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("VrInstance");

	public:

		Instance() noexcept = default;
		Instance(const Instance& other) = delete;
		Instance(Instance&& other) noexcept = default;
		~Instance() noexcept;

		Instance& operator = (const Instance& other) = delete;
		Instance& operator = (Instance&& other) noexcept = default;
		
		static auto GetGlfwRequiredInstanceExtensions();

		bool create();

		bool isValid() const noexcept { return instance; }

		void destroy() noexcept;

		const auto get() const noexcept { return instance; }
		auto get() noexcept { return instance; }

		void setEnableValidationLayers(bool value) noexcept { enableValidationLayers = value; }
		bool getEnableValidationLayers() const noexcept { return enableValidationLayers; }

		std::vector<const char*> getEnabledLayerNames() const noexcept;

		bool areEnabledLayersSupported() const noexcept;

		std::vector<const char*> getRequiredExtensions() const noexcept;

	protected:

		VkInstance instance = nullptr;
		bool enableValidationLayers = false;

	};

	inline auto Instance::GetGlfwRequiredInstanceExtensions()
	{
		std::uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;

		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		std::vector<const char*> result;
		for (std::uint32_t extensionIndex = 0; extensionIndex < glfwExtensionCount; ++extensionIndex)
		{
			result.push_back(glfwExtensions[extensionIndex]);
		}
		return result;
	}
}