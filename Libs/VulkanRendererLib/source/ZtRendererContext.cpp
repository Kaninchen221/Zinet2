#include "Zinet/VulkanRenderer/ZtRendererContext.hpp"

#include <vulkan/vulkan.h>

#include "Zinet/Window/ZtWindow.hpp"

namespace zt::vulkan_renderer
{
	bool DisplayImage::create(RendererContext& rendererContext, VkImage swapChainImage)
	{
		auto& swapChain = rendererContext.swapChain;
		auto& device = rendererContext.device;
		auto& renderPass = rendererContext.renderPass;

		image = swapChainImage;

		const auto imageViewCreateInfo = ImageView::GetDefaultCreateInfo(image, swapChain.getFormat());
		if (!imageView.create(device, imageViewCreateInfo))
		{
			Logger->error("Couldn't create an image view from one of the swap chain images");
			return false;
		}

		const auto swapChainSize = swapChain.getExtent();
		const Vector2ui framebufferSize{ swapChainSize.width, swapChainSize.height };
		if (!framebuffer.create(device, renderPass, imageView, framebufferSize))
		{
			Logger->error("Couldn't create framebuffer from one of the swap chain images");
			return false;
		}

		return true;
	}

	void DisplayImage::destroy(RendererContext& rendererContext)
	{
		auto& device = rendererContext.device;

		image = nullptr;
		framebuffer.destroy(device);
		imageView.destroy(device);
	}

	bool RendererContext::createDisplayImages()
	{
		auto swapChainImages = swapChain.getImages(device);
		if (swapChainImages.empty())
		{
			Logger->error("SwapChain::getImages returned empty container");
			return false;
		}

		displayImages.clear();
		displayImages.shrink_to_fit();
		displayImages.resize(swapChainImages.size());

		size_t imageIndex = 0;
		for (auto& displayImage : displayImages)
		{
			if (!displayImage.create(*this, swapChainImages[imageIndex]))
			{
				Logger->error("Couldn't create a display image of the index: {}", imageIndex);
				return false;
			}
			++imageIndex;
		}

		return true;
	}

	bool RendererContext::create(wd::Window& window)
	{
		if (!instance.create())
			return false;

		if (instance.getEnableValidationLayers())
		{
			if (!debugUtilsMessenger.create(instance))
				return false;
		}
		else
		{
			Logger->info("Skip creating debug utils messenger because instance has enable validation layers = false");
		}

		auto physicalDevices = instance.getPhysicalDevices();
		if (physicalDevices.empty())
		{
			Logger->error("Couldn't get any physical devices");
			return false;
		}

		physicalDevice = PhysicalDevice::TakeBestPhysicalDevice(physicalDevices);
		invalidateAll(physicalDevices);
		if (!physicalDevice.isValid())
			return false;

		if (!surface.create(instance, window))
			return false;

		if (!device.create(instance, physicalDevice, surface))
			return false;

		if (!vma.create(device, physicalDevice, instance))
			return false;

		if (!swapChain.create(device, physicalDevice, surface, window.getFramebufferSize()))
			return false;

		queue = device.getQueue();
		if (!queue.isValid())
			return false;

		if (!commandPool.create(device, queue))
			return false;

		if (!fence.create(device, true))
			return false;

		const auto renderPassCreateInfo = RenderPass::GetPresentCreateInfo(swapChain.getFormat());
		if (!renderPass.create(device, renderPassCreateInfo))
			return false;

		if (!createDisplayImages())
		{
			Logger->error("Couldn't create display images");
			return false;
		}

		if (!imageAvailableSemaphore.create(device))
			return false;

		if (!renderFinishedSemaphore.create(device))
			return false;

		return true;
	}

	void RendererContext::destroy()
	{
		renderFinishedSemaphore.destroy(device);
		imageAvailableSemaphore.destroy(device);

		for (auto& displayImage : displayImages)
		{
			displayImage.destroy(*this);
		}

		renderPass.destroy(device);
		fence.destroy(device);
		commandPool.destroy(device);
		swapChain.destroy(device);
		vma.destroy();
		queue.invalidate();
		device.destroy();
		physicalDevice.invalidate();
		surface.destroy(instance);
		debugUtilsMessenger.destroy(instance);
		instance.destroy();
	}

	void RendererContext::windowResized(const Vector2i& size)
	{
		renderPass.destroy(device);
		if (!renderPass.recreate(device))
		{
			Logger->error("Couldn't recreate render pass after window resized");
			return;
		}

		swapChain.destroy(device);
		if (!swapChain.create(device, physicalDevice, surface, size))
		{
			Logger->error("Couldn't recreate swap chain after window resized");
			return;
		}

		createDisplayImages();
	}

}