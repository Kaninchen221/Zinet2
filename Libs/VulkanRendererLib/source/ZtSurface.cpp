#include "Zinet/VulkanRenderer/ZtSurface.hpp"
#include "Zinet/VulkanRenderer/ZtInstance.hpp"

#include "Zinet/Window/ZtWindow.hpp"

namespace zt::vulkan_renderer
{
	bool Surface::create(const Instance& instance, wd::Window& window) ZINET_API_POST
	{
		if (isValid())
			return false;

		VkResult result = glfwCreateWindowSurface(instance.get(), window.getInternal(), nullptr, &objectHandle);
		if (result == VK_SUCCESS)
		{
			return true;
		}
		else
		{
			Logger->error("Can't create surface, result: {}", static_cast<std::int32_t>(result));
			return false;
		}
	}

	void Surface::destroy(const Instance& instance) ZINET_API_POST
	{
		if (isValid())
		{
			vkDestroySurfaceKHR(instance.get(), objectHandle, nullptr);
			objectHandle = nullptr;
		}
	}
}