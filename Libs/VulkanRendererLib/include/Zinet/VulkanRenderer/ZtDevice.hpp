#pragma once

#include "Zinet/VulkanRenderer/ZtVulkanRendererConfig.hpp"
#include "Zinet/VulkanRenderer/ZtVulkanObject.hpp"

#include "Zinet/Core/ZtLogger.hpp"

#include <vulkan/vulkan.h>

namespace zt::vulkan_renderer
{
	class ZINET_VULKAN_RENDERER_API Device : public VulkanObject<VkDevice>
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("VRDevice");

	public:

		Device(HandleType newObjectHandle)
			: VulkanObject(newObjectHandle) {}

		Device() noexcept = delete;
		Device(const Device& other) noexcept = delete;
		Device(Device&& other) noexcept = default;
		~Device() noexcept = default;

		Device& operator = (const Device& other) noexcept = delete;
		Device& operator = (Device&& other) noexcept = default;

		void destroy() noexcept;
	};
}