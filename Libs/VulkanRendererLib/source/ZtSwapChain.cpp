#include "Zinet/VulkanRenderer/ZtSwapChain.hpp"
#include "Zinet/VulkanRenderer/ZtDevice.hpp"
#include "Zinet/VulkanRenderer/ZtPhysicalDevice.hpp"
#include "Zinet/VulkanRenderer/ZtSurface.hpp"
#include "Zinet/VulkanRenderer/ZtSemaphore.hpp"

#include "Zinet/Math/ZtVecTypes.hpp"

#include "Zinet/Window/ZtWindow.hpp"

#include <algorithm>

#include "Zinet/Core/ZtUtils.hpp"

namespace zt::vulkan_renderer
{
	bool SwapChain::create(const Device& device, const PhysicalDevice& physicalDevice, const Surface& surface, const Vector2i& windowFramebufferSize)
	{
		if (isValid())
			return false;

		const auto surfaceCapabilities = physicalDevice.getPhysicalDeviceSurfaceCapabilities(surface);
		const auto availableFormats = physicalDevice.getPhysicalDeviceSurfaceFormats(surface);
		const auto presentModes = physicalDevice.getPhysicalDeviceSurfacePresentModes(surface);

		std::vector<VkSurfaceFormatKHR> correctSurfaceFormats{ { VK_FORMAT_B8G8R8A8_SRGB, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR } };
		auto getBestSurfaceFormat = [&correctSurfaceFormats = correctSurfaceFormats, &availableFormats = availableFormats]() -> std::optional<VkSurfaceFormatKHR>
		{
			for (auto correctSurfaceFormat : correctSurfaceFormats)
			{
				for (const auto& availableFormat : availableFormats)
				{
					if (availableFormat.format == correctSurfaceFormat.format && availableFormat.colorSpace == correctSurfaceFormat.colorSpace)
						return correctSurfaceFormat;
				}
			}

			return std::nullopt;
		};
		auto bestSurfaceFormat = getBestSurfaceFormat();
		if (!bestSurfaceFormat)
		{
			Logger->error("Physical device doesn't support format B8G8R8A8_SRGB and color space SRGB NONLINEAR");
			return false;
		}

		// Present Mode
		const bool supportsMailbox = core::Contains(presentModes, VK_PRESENT_MODE_MAILBOX_KHR);
		const auto presentMode = supportsMailbox ? VK_PRESENT_MODE_MAILBOX_KHR : VK_PRESENT_MODE_FIFO_KHR;

		// Extent
		VkExtent2D newExtent{ static_cast<std::uint32_t>(windowFramebufferSize.x), static_cast<std::uint32_t>(windowFramebufferSize.y) };
		newExtent.width = std::clamp(newExtent.width, surfaceCapabilities.minImageExtent.width, surfaceCapabilities.maxImageExtent.width);
		newExtent.height = std::clamp(newExtent.height, surfaceCapabilities.minImageExtent.height, surfaceCapabilities.maxImageExtent.height);

		const VkSwapchainCreateInfoKHR createInfo
		{
			.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
			.pNext = nullptr,
			.flags = {},
			.surface = surface.get(),
			.minImageCount = 
				std::clamp(surfaceCapabilities.minImageCount + 1, surfaceCapabilities.minImageCount, 
					std::max(surfaceCapabilities.minImageCount, surfaceCapabilities.maxImageCount)),
			.imageFormat = bestSurfaceFormat->format,
			.imageColorSpace = bestSurfaceFormat->colorSpace,
			.imageExtent = newExtent,
			.imageArrayLayers = 1,
			.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
			.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
			.queueFamilyIndexCount = 0,
			.pQueueFamilyIndices = nullptr,
			.preTransform = surfaceCapabilities.currentTransform,
			.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
			.presentMode = presentMode,
			.clipped = VK_TRUE,
			.oldSwapchain = nullptr
		};

		const auto createResult = vkCreateSwapchainKHR(device.get(), &createInfo, nullptr, &objectHandle);
		if (createResult == VK_SUCCESS)
		{
			extent = newExtent;
			format = bestSurfaceFormat->format;
			return true;
		}
		else
		{
			Logger->error("Can't create swap chain, result: {}", static_cast<std::int32_t>(createResult));
			return false;
		}
	}

	void SwapChain::destroy(const Device& device) noexcept
	{
		if (isValid())
		{
			vkDestroySwapchainKHR(device.get(), objectHandle, nullptr);
			objectHandle = nullptr;
		}
	}

	std::vector<VkImage> SwapChain::getImages(const Device& device) const
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

	std::uint32_t SwapChain::acquireNextImage(const Device& device, Semaphore& semaphore) const
	{
		std::uint32_t imageIndex{};
		// TODO: Fix validation layers error about semaphore on AMD integrated GPU (on laptop)
		const auto result =
			vkAcquireNextImageKHR(device.get(), objectHandle, std::numeric_limits<uint64_t>::max(), semaphore.get(), VK_NULL_HANDLE, &imageIndex);

		if (result == VK_SUCCESS)
		{
			return imageIndex;
		}
		else
		{
			Logger->error("Couldn't acquire next image, result: {}", static_cast<std::uint32_t>(result));
			return InvalidIndex;
		}
	}

}