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

	class  Surface : public VulkanObject<VkSurfaceKHR>
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("VRSurface");

	public:

		Surface(HandleType newObjectHandle)
			: VulkanObject(newObjectHandle) {}

		Surface() noexcept = delete;
		Surface(const Surface& other) noexcept = delete;
		Surface(Surface&& other) noexcept = default;
		~Surface() noexcept = default;

		Surface& operator = (const Surface& other) noexcept = delete;
		Surface& operator = (Surface&& other) noexcept = default;

		bool create(const Instance& instance, wd::Window& window);

		void destroy(const Instance& instance) noexcept;

	};
}