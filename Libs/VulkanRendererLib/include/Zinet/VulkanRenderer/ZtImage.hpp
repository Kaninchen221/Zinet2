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

		Image(HandleType newObjectHandle, VkFormat newFormat = VK_FORMAT_UNDEFINED)
			: VulkanObject{ newObjectHandle },
			format{ newFormat }
		{}

		Image() ZINET_API_POST = delete;
		Image(const Image& other) ZINET_API_POST = delete;
		Image(Image&& other) ZINET_API_POST = default;
		~Image() ZINET_API_POST = default;

		Image& operator = (const Image& other) ZINET_API_POST = delete;
		Image& operator = (Image&& other) ZINET_API_POST = default;

		static VkImageCreateInfo GetDefaultCreateInfo(const Device& device) ZINET_API_POST;

		bool create(const VMA& vma, const VkImageCreateInfo& createInfo) ZINET_API_POST;

		void destroy(const VMA& vma) ZINET_API_POST;

		VkFormat getFormat() const ZINET_API_POST { return format; }

		static VkImageSubresourceRange GetDefaultSubresourceRange() ZINET_API_POST;

		VkImageMemoryBarrier getDefaultMemoryBarier(
			VkImageLayout oldLayout = VK_IMAGE_LAYOUT_UNDEFINED,
			VkImageLayout newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			VkImageSubresourceRange subresourceRange = GetDefaultSubresourceRange()
			) const ZINET_API_POST;

	protected:

		VmaAllocation allocation{};
		VmaAllocationInfo allocationInfo{};

		VkFormat format = VK_FORMAT_UNDEFINED;

	};
}