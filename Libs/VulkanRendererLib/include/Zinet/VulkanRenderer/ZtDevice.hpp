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

		Device(HandleType newObjectHandle, std::uint32_t newPresentQueueFamilyIndex = InvalidIndex, std::uint32_t newSurfaceQueueFamilyIndex = InvalidIndex)
			: VulkanObject(newObjectHandle),
			  presentQueueFamilyIndex(newPresentQueueFamilyIndex),
			  surfaceQueueFamilyIndex(newSurfaceQueueFamilyIndex)
		{}

		Device() noexcept = delete;
		Device(const Device& other) noexcept = delete;
		Device(Device&& other) noexcept = default;
		~Device() noexcept = default;

		Device& operator = (const Device& other) noexcept = delete;
		Device& operator = (Device&& other) noexcept = default;

		void destroy() noexcept;

		Queue getPresentQueue() noexcept;

		Queue getSurfaceQueue() noexcept;

		std::int32_t getPresentQueueFamilyIndex() const noexcept { return presentQueueFamilyIndex; }

		std::int32_t getSurfaceQueueFamilyIndex() const noexcept { return surfaceQueueFamilyIndex; }

	protected:

		std::int32_t presentQueueFamilyIndex = InvalidIndex;
		std::int32_t surfaceQueueFamilyIndex = InvalidIndex;

	};
}