#include "Zinet/VulkanRenderer/ZtSurface.hpp"
#include "Zinet/VulkanRenderer/ZtInstance.hpp"

namespace zt::vulkan_renderer
{

	void Surface::destroy(Instance& instance) noexcept
	{
		if (isValid())
		{
			vkDestroySurfaceKHR(instance.get(), objectHandle, nullptr);
			objectHandle = nullptr;
		}
	}

}