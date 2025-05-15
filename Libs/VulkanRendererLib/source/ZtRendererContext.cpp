#include "Zinet/VulkanRenderer/ZtRendererContext.hpp"

#include <vulkan/vulkan.h>

#include "Zinet/Window/ZtWindow.hpp"

namespace zt::vulkan_renderer
{
	bool RendererContext::create(wd::Window& window) noexcept
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
		invalidateAll(physicalDevices);
		if (!physicalDevice.isValid())
			return false;

		if (!surface.create(instance, window))
			return false;

		if (!device.create(physicalDevice, surface))
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

		return true;
	}

	void RendererContext::destroy() noexcept
	{
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
}