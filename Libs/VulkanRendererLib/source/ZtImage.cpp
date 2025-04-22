#include "Zinet/VulkanRenderer/ZtImage.hpp"
#include "Zinet/VulkanRenderer/ZtDevice.hpp"
#include "Zinet/VulkanRenderer/ZtVMA.hpp"

#include <vulkan/vulkan.h>

namespace zt::vulkan_renderer
{
	VkImageCreateInfo Image::GetDefaultCreateInfo(const Device& device) noexcept
	{
		VkImageCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		createInfo.imageType = VK_IMAGE_TYPE_2D;
		createInfo.format = VK_FORMAT_B8G8R8A8_SRGB;
		createInfo.extent = VkExtent3D{ 200, 200, 1 };
		createInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		createInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
		createInfo.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		createInfo.samples = VK_SAMPLE_COUNT_4_BIT;
		createInfo.queueFamilyIndexCount = 1;
		createInfo.mipLevels = 1;
		createInfo.arrayLayers = 1;
		createInfo.pQueueFamilyIndices = &device.getQueueFamilyIndex();

		return createInfo;
	}

	bool Image::create(const VMA& vma, const VkImageCreateInfo& createInfo) noexcept
	{
		VmaAllocationCreateInfo allocationCreateInfo = {};
		allocationCreateInfo.usage = VMA_MEMORY_USAGE_AUTO;

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

	void Image::destroy(const VMA& vma) noexcept
	{
		if (isValid())
		{
			vmaDestroyImage(vma.get(), objectHandle, allocation);
			objectHandle = nullptr;
		}
	}

}