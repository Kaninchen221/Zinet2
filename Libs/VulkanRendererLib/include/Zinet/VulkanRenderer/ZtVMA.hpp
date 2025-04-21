#pragma once

#include "Zinet/VulkanRenderer/ZtVulkanRendererConfig.hpp"
#include "Zinet/VulkanRenderer/ZtVulkanObject.hpp"

#include "Zinet/Core/ZtLogger.hpp"

#define VMA_STATIC_VULKAN_FUNCTIONS 0
#define VMA_DYNAMIC_VULKAN_FUNCTIONS 1
#define VMA_IMPLEMENTATION
#define VMA_VULKAN_VERSION 1002000
#include <vk_mem_alloc.h>

namespace zt::vulkan_renderer
{
	class Device;
	class PhysicalDevice;
	class Instance;

	class ZINET_VULKAN_RENDERER_API VMA : public VulkanObject<VmaAllocator>
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("VR_VMA");

	public:

		VMA(HandleType newObjectHandle)
			: VulkanObject(newObjectHandle) {}

		VMA() noexcept = delete;
		VMA(const VMA& other) noexcept = delete;
		VMA(VMA&& other) noexcept = default;
		~VMA() noexcept = default;

		VMA& operator = (const VMA& other) noexcept = delete;
		VMA& operator = (VMA&& other) noexcept = default;

		bool create(const Device& device, const PhysicalDevice& physicalDevice, const Instance& instance) noexcept;

		void destroy() noexcept;
	};
}