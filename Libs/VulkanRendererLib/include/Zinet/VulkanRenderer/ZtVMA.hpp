#pragma once

#include "Zinet/VulkanRenderer/ZtVulkanRendererConfig.hpp"
#include "Zinet/VulkanRenderer/ZtVulkanObject.hpp"

#include "Zinet/Core/ZtLogger.hpp"

#define VMA_STATIC_VULKAN_FUNCTIONS 0
#define VMA_DYNAMIC_VULKAN_FUNCTIONS 1

#undef VMA_VULKAN_VERSION
#define VMA_VULKAN_VERSION 1002000

#if ZINET_LINUX
#	define VMA_MEMORY_BUDGET 0 // Not available feature on linux build machine
#endif

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

		VMA() ZINET_API_POST = delete;
		VMA(const VMA& other) ZINET_API_POST = delete;
		VMA(VMA&& other) ZINET_API_POST = default;
		~VMA() ZINET_API_POST = default;

		VMA& operator = (const VMA& other) ZINET_API_POST = delete;
		VMA& operator = (VMA&& other) ZINET_API_POST = default;

		bool create(const Device& device, const PhysicalDevice& physicalDevice, const Instance& instance) ZINET_API_POST;

		void destroy() ZINET_API_POST;
	};
}