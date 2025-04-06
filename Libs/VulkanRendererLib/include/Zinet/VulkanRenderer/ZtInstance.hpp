#pragma once

#include "Zinet/VulkanRenderer/ZtVulkanRendererConfig.hpp"
#include "Zinet/VulkanRenderer/ZtVulkanObject.hpp"
#include "Zinet/VulkanRenderer/ZtPhysicalDevice.hpp"

#include "Zinet/Core/ZtLogger.hpp"

#include <vulkan/vulkan.h>

namespace zt::vulkan_renderer
{
	class ZINET_VULKAN_RENDERER_API Instance : public VulkanObject<VkInstance>
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("VRInstance");

	public:

		Instance() noexcept = default;
		Instance(const Instance& other) noexcept = delete;
		Instance(Instance&& other) noexcept = default;
		~Instance() noexcept = default;

		Instance& operator = (const Instance& other) noexcept = delete;
		Instance& operator = (Instance&& other) noexcept = default;

		bool create() noexcept;

		void destroy() noexcept;

		void setEnableValidationLayers(bool value) noexcept { enableValidationLayers = value; }
		bool getEnableValidationLayers() const noexcept { return enableValidationLayers; }

		std::vector<const char*> getEnabledLayerNames() const noexcept;

		bool areEnabledLayersSupported() const noexcept;

		static std::vector<const char*> GetGlfwRequiredInstanceExtensions() noexcept;

		std::vector<const char*> getRequiredExtensions() const noexcept;

		std::vector<PhysicalDevice> createPhysicalDevices() const noexcept;

	protected:

		bool enableValidationLayers = false;

	};
}