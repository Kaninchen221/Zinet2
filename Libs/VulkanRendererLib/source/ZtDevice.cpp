#include "Zinet/VulkanRenderer/ZtDevice.hpp"
#include "Zinet/VulkanRenderer/ZtSurface.hpp"
#include "Zinet/VulkanRenderer/ZtPhysicalDevice.hpp"

#include "Zinet/Core/ZtUtils.hpp"

#include "Zinet/Window/ZtWindow.hpp"

namespace zt::vulkan_renderer
{
	void Device::destroy() noexcept
	{
		if (isValid())
		{
			vkDestroyDevice(objectHandle, nullptr);
			objectHandle = nullptr;
		}
	}

	Queue Device::getQueue() noexcept
	{
		if (queueFamilyIndex == InvalidIndex)
			return Queue{ nullptr };

		VkQueue queueObjectHandle = nullptr;
		vkGetDeviceQueue(objectHandle, queueFamilyIndex, 0, &queueObjectHandle);

		return Queue(queueObjectHandle);
	}

	SwapChain Device::createSwapChain(const PhysicalDevice& physicalDevice, const Surface& surface, const wd::Window& window) noexcept
	{
		const auto surfaceCapabilities = physicalDevice.getPhysicalDeviceSurfaceCapabilities(surface);
		const auto availableFormats = physicalDevice.getPhysicalDeviceSurfaceFormats(surface);
		const auto presentModes = physicalDevice.getPhysicalDeviceSurfacePresentModes(surface);

		// Format and Color Space
		Logger->info("Print formats");
		for (const auto& availableFormat : availableFormats)
		{
			Logger->info("\tFormat: {}, Color Space: {}", 
				static_cast<std::int32_t>(availableFormat.format), static_cast<std::int32_t>(availableFormat.colorSpace));
		}

		const auto format = VK_FORMAT_B8G8R8A8_SRGB;
		const auto colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
		const bool supportsFormat = [availableFormats](VkFormat format, VkColorSpaceKHR colorSpace) -> bool
		{
			for (const auto& availableFormat : availableFormats)
			{
				if (availableFormat.format == format && availableFormat.colorSpace == colorSpace)
					return true;
			}

			return false;
		}(format, colorSpace);
		if (!supportsFormat)
		{
			Logger->error("Physical device doesn't support format B8G8R8A8_SRGB and color space SRGB NONLINEAR");
			return SwapChain{ nullptr };
		}

		// Present Mode
		const bool supportsMailbox = core::Contains(presentModes, VK_PRESENT_MODE_MAILBOX_KHR);
		const auto presentMode = supportsMailbox ? VK_PRESENT_MODE_MAILBOX_KHR : VK_PRESENT_MODE_FIFO_KHR;

		// Extent
		Vector2ui framebufferSize = window.getFramebufferSize();
		VkExtent2D extent{ framebufferSize.x, framebufferSize.y };
		extent.width = std::clamp(extent.width, surfaceCapabilities.minImageExtent.width, surfaceCapabilities.maxImageExtent.width);
		extent.height = std::clamp(extent.height, surfaceCapabilities.minImageExtent.height, surfaceCapabilities.maxImageExtent.height);

		VkSwapchainCreateInfoKHR createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.minImageCount = std::clamp(surfaceCapabilities.minImageCount + 1, surfaceCapabilities.minImageCount, surfaceCapabilities.maxImageCount);
		createInfo.imageFormat = format;
		createInfo.imageColorSpace = colorSpace;
		createInfo.presentMode = presentMode;
		createInfo.surface = surface.get();
		createInfo.preTransform = surfaceCapabilities.currentTransform;
		createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR; // TODO: Blend with other windows behind our window? Will be useful for the final tech demo.
		createInfo.imageExtent = extent;
		createInfo.imageArrayLayers = 1;
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		createInfo.queueFamilyIndexCount = 0; // Optional
		createInfo.pQueueFamilyIndices = nullptr; // Optional
		createInfo.clipped = VK_TRUE;
		createInfo.oldSwapchain = nullptr;

		VkSwapchainKHR swapChainObjectHandle = nullptr;
		const auto createResult = vkCreateSwapchainKHR(objectHandle, &createInfo, nullptr, &swapChainObjectHandle);

		if (createResult == VK_SUCCESS)
		{
			return SwapChain{ swapChainObjectHandle };
		}
		else
		{
			Logger->error("Can't create swap chain, result: {}", static_cast<std::int32_t>(createResult));
			return SwapChain{ nullptr };
		}
	}

}