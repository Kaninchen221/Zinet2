#pragma once

#include "Zinet/VulkanRenderer/ZtVulkanRendererConfig.hpp"
#include "Zinet/VulkanRenderer/ZtVulkanObject.hpp"

#include "Zinet/Core/ZtLogger.hpp"

#include <vulkan/vulkan.h>

namespace zt::wd
{
	class Window;
}

namespace zt::vulkan_renderer
{
	class Instance;

	class ZINET_VULKAN_RENDERER_API Surface : public VulkanObject<VkSurfaceKHR>
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("VRSurface");

	public:

		Surface(HandleType newObjectHandle)
			: VulkanObject(newObjectHandle) {}

		Surface() ZINET_API_POST = delete;
		Surface(const Surface& other) ZINET_API_POST = delete;
		Surface(Surface&& other) ZINET_API_POST = default;
		~Surface() ZINET_API_POST = default;

		Surface& operator = (const Surface& other) ZINET_API_POST = delete;
		Surface& operator = (Surface&& other) ZINET_API_POST = default;

		bool create(const Instance& instance, wd::Window& window) ZINET_API_POST;

		void destroy(const Instance& instance) ZINET_API_POST;

	};
}