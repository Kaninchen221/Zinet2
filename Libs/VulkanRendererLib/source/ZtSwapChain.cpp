#include "Zinet/VulkanRenderer/ZtSwapChain.hpp"
#include "Zinet/VulkanRenderer/ZtDevice.hpp"

namespace zt::vulkan_renderer
{

	void SwapChain::destroy(Device& device) noexcept
	{
		if (isValid())
		{
			vkDestroySwapchainKHR(device.get(), objectHandle, nullptr);
			objectHandle = nullptr;
		}
	}

	std::vector<VkImage> SwapChain::getImages(Device& device) noexcept
	{
		std::uint32_t count;
		vkGetSwapchainImagesKHR(device.get(), objectHandle, &count, nullptr);

		if (count == 0)
		{
			Logger->error("Surface get images return no images");
			return {};
		}

		std::vector<VkImage> images;
		images.resize(count);
		vkGetSwapchainImagesKHR(device.get(), objectHandle, &count, images.data());
		return images;
	}

}