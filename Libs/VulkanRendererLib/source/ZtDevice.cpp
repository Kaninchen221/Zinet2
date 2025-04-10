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

	Queue Device::getQueue() noexcept
	{
		VkQueue queueObjectHandle = nullptr;
		vkGetDeviceQueue(objectHandle, queueFamilyIndex, 0, &queueObjectHandle);

		return Queue(queueObjectHandle);
	}
}