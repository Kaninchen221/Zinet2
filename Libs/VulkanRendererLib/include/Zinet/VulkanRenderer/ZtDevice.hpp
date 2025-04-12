#pragma once

#include "Zinet/VulkanRenderer/ZtVulkanRendererConfig.hpp"
#include "Zinet/VulkanRenderer/ZtVulkanObject.hpp"
#include "Zinet/VulkanRenderer/ZtQueue.hpp"

#include "Zinet/Core/ZtLogger.hpp"

#include <vulkan/vulkan.h>

namespace zt::vulkan_renderer
{
	class ZINET_VULKAN_RENDERER_API Device : public VulkanObject<VkDevice>
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("VRDevice");

	public:

		Device(HandleType newObjectHandle, std::uint32_t newQueueFamilyIndex = InvalidIndex)
			: VulkanObject(newObjectHandle),
			queueFamilyIndex(newQueueFamilyIndex)
		{}

		Device() noexcept = delete;
		Device(const Device& other) noexcept = delete;
		Device(Device&& other) noexcept = default;
		~Device() noexcept = default;

		Device& operator = (const Device& other) noexcept = delete;
		Device& operator = (Device&& other) noexcept = default;

		void destroy() noexcept;

		Queue getQueue() noexcept;

		std::int32_t getQueueFamilyIndex() const noexcept { return queueFamilyIndex; }

	protected:

		std::int32_t queueFamilyIndex = InvalidIndex;

	};
}