#include "Zinet/VulkanRenderer/ZtSurface.hpp"
#include "Zinet/VulkanRenderer/ZtInstance.hpp"

#include "Zinet/Window/ZtWindow.hpp"

namespace zt::vulkan_renderer
{
	bool Surface::create(const Instance& instance, wd::Window& window)
	{
		if (isValid())
			return false;

		VkResult result = glfwCreateWindowSurface(instance.get(), window.getInternal(), nullptr, &get());
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

	void Surface::destroy(const Instance& instance) noexcept
	{
		if (isValid())
		{
			vkDestroySurfaceKHR(instance.get(), get(), nullptr);
			invalidateInternal();
		}
	}
}