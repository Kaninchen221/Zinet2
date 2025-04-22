#pragma once

#include "Zinet/VulkanRenderer/ZtVulkanRendererConfig.hpp"
#include "Zinet/VulkanRenderer/ZtVulkanObject.hpp"

#include "Zinet/Core/ZtLogger.hpp"

#include <vulkan/vulkan.h>

#include <vk_mem_alloc.h>

namespace zt::vulkan_renderer
{
	class Device;
	class VMA;

	class ZINET_VULKAN_RENDERER_API Image : public VulkanObject<VkImage>
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("VRImage");

	public:

		Image(HandleType newObjectHandle)
			: VulkanObject(newObjectHandle)
		{}

		Image() noexcept = delete;
		Image(const Image& other) noexcept = delete;
		Image(Image&& other) noexcept = default;
		~Image() noexcept = default;

		Image& operator = (const Image& other) noexcept = delete;
		Image& operator = (Image&& other) noexcept = default;

		static VkImageCreateInfo GetDefaultCreateInfo(const Device& device) noexcept;

		bool create(const VMA& vma, const VkImageCreateInfo& createInfo) noexcept;

		void destroy(const VMA& vma) noexcept;

		VkFormat getFormat() const noexcept { return format; }

	protected:

		VmaAllocation allocation{};
		VmaAllocationInfo allocationInfo{};

		VkFormat format = VK_FORMAT_UNDEFINED;

	};
}