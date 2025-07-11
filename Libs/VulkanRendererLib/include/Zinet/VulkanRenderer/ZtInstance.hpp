#pragma once

#include "Zinet/VulkanRenderer/ZtVulkanRendererConfig.hpp"
#include "Zinet/VulkanRenderer/ZtVulkanObject.hpp"
#include "Zinet/VulkanRenderer/ZtPhysicalDevice.hpp"
#include "Zinet/VulkanRenderer/ZtSurface.hpp"

#include "Zinet/Core/ZtLogger.hpp"

#include <vulkan/vulkan.h>

namespace zt::wd
{
	class Window;
}

namespace zt::vulkan_renderer
{
	class ZINET_VULKAN_RENDERER_API Instance : public VulkanObject<VkInstance>
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("VRInstance");

	public:

		Instance(HandleType newObjectHandle)
			: VulkanObject(newObjectHandle) {}

		Instance() ZINET_API_POST = delete;
		Instance(const Instance& other) ZINET_API_POST = delete;
		Instance(Instance&& other) ZINET_API_POST = default;
		~Instance() ZINET_API_POST = default;

		Instance& operator = (const Instance& other) ZINET_API_POST = delete;
		Instance& operator = (Instance&& other) ZINET_API_POST = default;

		static void PrintAPIVersion() ZINET_API_POST;

		bool create() ZINET_API_POST;

		void destroy() ZINET_API_POST;

		void setEnableValidationLayers(bool value) ZINET_API_POST { enableValidationLayers = value; }
		bool getEnableValidationLayers() const ZINET_API_POST { return enableValidationLayers; }

		std::vector<const char*> getEnabledLayerNames() const ZINET_API_POST;

		bool areEnabledLayersSupported() const ZINET_API_POST;

		static std::vector<const char*> GetGlfwRequiredInstanceExtensions() ZINET_API_POST;

		std::vector<const char*> getRequiredExtensions() const ZINET_API_POST;

		std::vector<PhysicalDevice> getPhysicalDevices() const ZINET_API_POST;

	protected:

		bool enableValidationLayers = false;

	};
}