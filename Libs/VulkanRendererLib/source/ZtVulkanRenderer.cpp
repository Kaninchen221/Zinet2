#include "Zinet/VulkanRenderer/ZtVulkanRenderer.hpp"

#include "Zinet/Window/ZtWindow.hpp"

namespace zt::vulkan_renderer
{

	bool VulkanRenderer::start(wd::Window& window)
	{
		instance.setEnableValidationLayers(true);
		if (!instance.create())
			return false;

		if (!debugUtilsMessenger.create(instance))
			return false;

		auto physicalDevices = instance.getPhysicalDevices();
		if (physicalDevices.empty())
		{
			Logger->error("Couldn't get any physical devices");
			return false;
		}

		physicalDevice = PhysicalDevice::TakeBestPhysicalDevice(physicalDevices);
		if (!physicalDevice.isValid())
			return false;

		if (!surface.create(instance, window))
			return false;

		if (!device.create(physicalDevice, surface))
			return false;

		if (!swapChain.create(device, physicalDevice, surface, window))
			return false;

		images = swapChain.getImages(device);
		if (images.empty())
		{
			Logger->error("SwapChain returned empty swapChainImages");
			return false;
		}

		for (auto image : images)
		{
			auto& imageView = imageViews.emplace_back(nullptr);
			if (!imageView.create(image, swapChain.getFormat(), device))
			{
				Logger->error("Couldn't create image view from one of the swap chain images");
				return false;
			}
		}

		if (!renderPass.createForPresent(device, swapChain.getFormat()))
			return false;

		const auto windowSize = window.getSize();
		for (auto& imageView : imageViews)
		{
			auto& framebuffer = framebuffers.emplace_back(nullptr);
			if (!framebuffer.create(device, renderPass, imageView, windowSize))
			{
				Logger->error("Couldn't create image view from one of the swap chain images");
				return false;
			}
		}

		return true;
	}

	void VulkanRenderer::shutdown()
	{
		for (auto& framebuffer : framebuffers)
			framebuffer.destroy(device);

		renderPass.destroy(device);

		for (auto& imageView : imageViews)
			imageView.destroy(device);

		swapChain.destroy(device);
		device.destroy();
		surface.destroy(instance);
		debugUtilsMessenger.destroy(instance);
		instance.destroy();
	}

}