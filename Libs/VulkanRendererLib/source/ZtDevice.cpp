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

}