#include "Zinet/VulkanRenderer/ZtImage.hpp"
#include "Zinet/VulkanRenderer/ZtDevice.hpp"
#include "Zinet/VulkanRenderer/ZtVMA.hpp"

#include <vulkan/vulkan.h>

namespace zt::vulkan_renderer
{
	VkImageCreateInfo Image::GetDefaultCreateInfo(const Device& device) ZINET_API_POST
	{
		return VkImageCreateInfo
		{
			.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
			.pNext = nullptr,
			.flags = {},
			.imageType = VK_IMAGE_TYPE_2D,
			.format = VK_FORMAT_B8G8R8A8_SRGB,
			.extent = VkExtent3D{ 200, 200, 1 },
			.mipLevels = 1,
			.arrayLayers = 1,
			.samples = VK_SAMPLE_COUNT_4_BIT,
			.tiling = VK_IMAGE_TILING_OPTIMAL,
			.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
			.sharingMode = VK_SHARING_MODE_EXCLUSIVE,
			.queueFamilyIndexCount = 1,
			.pQueueFamilyIndices = &device.getQueueFamilyIndex(),
			.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED
		};
	}

	bool Image::create(const VMA& vma, const VkImageCreateInfo& createInfo) ZINET_API_POST
	{
		if (isValid())
			return false;

		const VmaAllocationCreateInfo allocationCreateInfo
		{
			.flags = {},
			.usage = VMA_MEMORY_USAGE_AUTO
		};

		const auto result = vmaCreateImage(vma.get(), &createInfo, &allocationCreateInfo, &objectHandle, &allocation, &allocationInfo);
		if (result == VK_SUCCESS)
		{
			format = createInfo.format;
			return true;
		}
		else
		{
			Logger->error("Couldn't create Image, result: {}", static_cast<std::int32_t>(result));
			return false;
		}
	}

	void Image::destroy(const VMA& vma) ZINET_API_POST
	{
		if (isValid())
		{
			vmaDestroyImage(vma.get(), objectHandle, allocation);
			objectHandle = nullptr;
		}
	}

	VkImageSubresourceRange Image::GetDefaultSubresourceRange() ZINET_API_POST
	{
		return
		{
			.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
			.baseMipLevel = 0,
			.levelCount = 1,
			.baseArrayLayer = 0,
			.layerCount = 1
		};
	}

	VkImageMemoryBarrier Image::getDefaultMemoryBarier(VkImageLayout oldLayout, VkImageLayout newLayout, VkImageSubresourceRange subresourceRange) const ZINET_API_POST
	{
		return 
		{
			.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
			.pNext = nullptr,
			.srcAccessMask = VK_ACCESS_NONE,
			.dstAccessMask = VK_ACCESS_NONE,
			.oldLayout = oldLayout,
			.newLayout = newLayout,
			.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
			.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
			.image = get(),
			.subresourceRange = subresourceRange
		};
	}
}