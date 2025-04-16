#include "Zinet/VulkanRenderer/ZtImageView.hpp"
#include "Zinet/VulkanRenderer/ZtDevice.hpp"

#include <vulkan/vulkan.h>

namespace zt::vulkan_renderer
{
	bool ImageView::createFromSwapChainImage(const VkImage& vkImage, VkFormat format, const Device& device)
	{
		VkImageViewCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		createInfo.image = vkImage;
		createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		createInfo.format = format;
		createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		createInfo.subresourceRange.baseMipLevel = 0;
		createInfo.subresourceRange.levelCount = 1;
		createInfo.subresourceRange.baseArrayLayer = 0;
		createInfo.subresourceRange.layerCount = 1;

		const auto createResult = vkCreateImageView(device.get(), &createInfo, nullptr, &objectHandle);
		if (createResult == VK_SUCCESS)
		{
			return true;
		}
		else
		{
			Logger->error("Can't ImageView, result: {}", static_cast<std::int32_t>(createResult));
			return false;
		}
	}

	void ImageView::destroy(const Device& device)
	{
		if (isValid())
		{
			vkDestroyImageView(device.get(), objectHandle, nullptr);
			objectHandle = nullptr;
		}
	}
}