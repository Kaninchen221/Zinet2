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

	class VMA : public VulkanObject<VmaAllocator>
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("VR_VMA");

	public:

		VMA(VulcanType newHandle)
			: VulkanObject(newHandle) {}

		VMA() noexcept = delete;
		VMA(const VMA& other) noexcept = delete;
		VMA(VMA&& other) noexcept = default;
		~VMA() noexcept = default;

		VMA& operator = (const VMA& other) noexcept = delete;
		VMA& operator = (VMA&& other) noexcept = default;

		bool create(const Device& device, const PhysicalDevice& physicalDevice, const Instance& instance);

		void destroy();

		void updateBuildStatsString() noexcept;
		std::string_view getBuildStatsString() const noexcept { return buildStatsString ? buildStatsString : ""; }

	private:

		void freeBuildStatsString() noexcept;
		char* buildStatsString = nullptr;

	};
}