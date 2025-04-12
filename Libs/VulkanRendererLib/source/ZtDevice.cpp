#include "Zinet/VulkanRenderer/ZtDevice.hpp"

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

	Queue Device::getPresentQueue() noexcept
	{
		if (presentQueueFamilyIndex == InvalidIndex)
			return Queue{ nullptr };

		VkQueue queueObjectHandle = nullptr;
		vkGetDeviceQueue(objectHandle, presentQueueFamilyIndex, 0, &queueObjectHandle);

		return Queue(queueObjectHandle);
	}

	Queue Device::getSurfaceQueue() noexcept
	{
		if (presentQueueFamilyIndex == InvalidIndex)
			return getPresentQueue();

		if (surfaceQueueFamilyIndex == InvalidIndex)
			return Queue{ nullptr };

		std::uint32_t queueIndex = presentQueueFamilyIndex == surfaceQueueFamilyIndex ? 1 : 0;

		VkQueue queueObjectHandle = nullptr;
		vkGetDeviceQueue(objectHandle, presentQueueFamilyIndex, queueIndex, &queueObjectHandle);

		return Queue(queueObjectHandle);
	}

}