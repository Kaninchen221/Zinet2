#pragma once

#include "Zinet/VulkanRenderer/ZtVulkanRendererConfig.hpp"
#include "Zinet/VulkanRenderer/ZtVulkanObject.hpp"
#include "Zinet/VulkanRenderer/ZtQueue.hpp"
#include "Zinet/VulkanRenderer/ZtSwapChain.hpp"

#include "Zinet/Core/ZtLogger.hpp"

#include <vulkan/vulkan.h>

namespace zt::wd
{
	class Window;
}

namespace zt::vulkan_renderer
{
	class Surface;
	class PhysicalDevice;

	class ZINET_VULKAN_RENDERER_API Device : public VulkanObject<VkDevice>
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("VRDevice");

	public:

		Device(HandleType newObjectHandle)
			: VulkanObject(newObjectHandle)
		{}

		Device() noexcept = delete;
		Device(const Device& other) noexcept = delete;
		Device(Device&& other) noexcept = default;
		~Device() noexcept = default;

		Device& operator = (const Device& other) noexcept = delete;
		Device& operator = (Device&& other) noexcept = default;

		bool create(const PhysicalDevice& physicalDevice, const Surface& surface) noexcept;

		void destroy() noexcept;

		Queue getQueue() noexcept;

		const auto& getQueueFamilyIndex() const noexcept { return queueFamilyIndex; }

	protected:

		std::uint32_t queueFamilyIndex = InvalidIndex;

	};
}