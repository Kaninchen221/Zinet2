#include "Zinet/VulkanRenderer/ZtImageView.hpp"
#include "Zinet/VulkanRenderer/ZtDevice.hpp"

#include <vulkan/vulkan.h>

namespace zt::vulkan_renderer
{
	VkImageViewCreateInfo ImageView::GetDefaultCreateInfo(const VkImage& vkImage, VkFormat format) noexcept
	{
		return VkImageViewCreateInfo
		{
			.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
			.pNext = nullptr,
			.flags = {},
			.image = vkImage,
			.viewType = VK_IMAGE_VIEW_TYPE_2D,
			.format = format,
			.components = 
			{ 
				VK_COMPONENT_SWIZZLE_IDENTITY, 
				VK_COMPONENT_SWIZZLE_IDENTITY, 
				VK_COMPONENT_SWIZZLE_IDENTITY, 
				VK_COMPONENT_SWIZZLE_IDENTITY 
			},
			.subresourceRange =
			{
				.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
				.baseMipLevel = 0,
				.levelCount = 1,
				.baseArrayLayer = 0,
				.layerCount = 1
			}
		};
	}

	bool ImageView::create(const Device& device, const VkImageViewCreateInfo& createInfo) noexcept
	{
		if (isValid())
			return false;

		const auto createResult = vkCreateImageView(device.get(), &createInfo, nullptr, &objectHandle);
		if (createResult == VK_SUCCESS)
		{
			format = createInfo.format;
			return true;
		}
		else
		{
			Logger->error("Can't ImageView, result: {}", static_cast<std::int32_t>(createResult));
			return false;
		}
	}

	void ImageView::destroy(const Device& device) noexcept
	{
		if (isValid())
		{
			vkDestroyImageView(device.get(), objectHandle, nullptr);
			objectHandle = nullptr;
		}
	}

}