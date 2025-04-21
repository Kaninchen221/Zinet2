#include "Zinet/VulkanRenderer/ZtInstance.hpp"

#include "Zinet/Core/ZtUtils.hpp"

#include "Zinet/Window/ZtWindow.hpp"

namespace zt::vulkan_renderer
{
	bool Instance::create() noexcept
	{
		VkApplicationInfo appInfo{};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "Zinet Vulkan Renderer";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 2, 0);
		appInfo.pEngineName = "Zinet";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_0;

		VkInstanceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;

		const auto enabledLayerNames = getEnabledLayerNames();
		createInfo.enabledLayerCount = static_cast<std::uint32_t>(enabledLayerNames.size());
		createInfo.ppEnabledLayerNames = enabledLayerNames.data();;

		const auto extensions = getRequiredExtensions();
		createInfo.enabledExtensionCount = static_cast<std::uint32_t>(extensions.size());
		createInfo.ppEnabledExtensionNames = extensions.data();

		VkResult result = vkCreateInstance(&createInfo, nullptr, &objectHandle);
		return result == VK_SUCCESS;
	}

	void Instance::destroy() noexcept
	{
		if (isValid())
		{
			vkDestroyInstance(objectHandle, nullptr);
			objectHandle = nullptr;
		}
	}

	std::vector<const char*> Instance::getEnabledLayerNames() const noexcept
	{
		if (enableValidationLayers)
			return { "VK_LAYER_KHRONOS_validation" };
		else
			return {};
	}

	bool Instance::areEnabledLayersSupported() const noexcept
	{
		// TODO: Refactor this
		std::uint32_t layerCount;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

		std::vector<VkLayerProperties> availableLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

		bool areAllLayersSupported = true;

		const auto enabledLayerNames = getEnabledLayerNames();
		for (auto enabledLayerName : enabledLayerNames)
		{
			bool found = false;

			for (const auto& availableLayer : availableLayers)
			{
				if (std::string_view(availableLayer.layerName) == std::string_view(enabledLayerName))
				{
					found = true;
					break;
				}
			}
			
			if (!found)
			{
				Logger->error("Layer: {} is not available", enabledLayerName);
				areAllLayersSupported = false;
			}
		}

		return areAllLayersSupported;
	}

	std::vector<const char*> Instance::GetGlfwRequiredInstanceExtensions() noexcept
	{
		if (!wd::GLFW::IsInitialized())
			return {};

		std::uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;

		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		std::vector<const char*> result;
		for (std::uint32_t extensionIndex = 0; extensionIndex < glfwExtensionCount; ++extensionIndex)
		{
			result.push_back(glfwExtensions[extensionIndex]);
		}
		return result;
	}

	std::vector<const char*> Instance::getRequiredExtensions() const noexcept
	{
		auto extensions = GetGlfwRequiredInstanceExtensions();

		if (enableValidationLayers)
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

		return extensions;
	}

	std::vector<PhysicalDevice> Instance::getPhysicalDevices() const noexcept
	{
		std::uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(objectHandle, &deviceCount, nullptr);

		std::vector<VkPhysicalDevice> rawDevices(deviceCount);
		vkEnumeratePhysicalDevices(objectHandle, &deviceCount, rawDevices.data());

		std::vector<PhysicalDevice> result;
		result.reserve(deviceCount);
		for (auto rawDevice : rawDevices)
		{
			result.emplace_back(rawDevice);
		}
		return result;
	}

}