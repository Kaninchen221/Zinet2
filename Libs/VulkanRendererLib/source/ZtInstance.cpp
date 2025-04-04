#include "Zinet/VulkanRenderer/ZtInstance.hpp"

namespace zt::vulkan_renderer
{
	bool Instance::create()
	{
		VkApplicationInfo appInfo{};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "Zinet Vulkan Renderer";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "Zinet";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_0;

		VkInstanceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;
		createInfo.enabledLayerCount = 0;

		const auto glfwExtensions = GetGlfwRequiredInstanceExtensions();

		createInfo.enabledExtensionCount = static_cast<std::uint32_t>(glfwExtensions.size());
		createInfo.ppEnabledExtensionNames = glfwExtensions.data();

		VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);
		return result == VK_SUCCESS;
	}

	Instance::~Instance() noexcept
	{
		invalidate();
	}

	void Instance::invalidate() noexcept
	{
		if (instance)
		{
			vkDestroyInstance(instance, nullptr);
			instance = nullptr;
		}
	}
}