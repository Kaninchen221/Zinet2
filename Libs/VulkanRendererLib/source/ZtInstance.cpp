#include "Zinet/VulkanRenderer/ZtInstance.hpp"

#include "Zinet/Core/ZtUtils.hpp"

#include "Zinet/Window/ZtWindow.hpp"

namespace zt::vulkan_renderer
{
	void Instance::PrintAPIVersion()
	{
		std::uint32_t apiVersion = 0;
		vkEnumerateInstanceVersion(&apiVersion);

		const auto major = VK_VERSION_MAJOR(apiVersion);
		const auto minor = VK_VERSION_MINOR(apiVersion);
		const auto patch = VK_VERSION_PATCH(apiVersion);

		Logger->info("API Version: {} {} {}", major, minor, patch);
	}

	bool Instance::create()
	{
		if (isValid())
			return false;

		const VkApplicationInfo applicationInfo
		{
			.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
			.pApplicationName = "Zinet Vulkan Renderer",
			.applicationVersion = VK_MAKE_VERSION(1, 0, 0),
			.pEngineName = "Zinet",
			.engineVersion = VK_MAKE_VERSION(1, 0, 0),
			.apiVersion = VK_API_VERSION_1_2
		};

		const auto enabledLayerNames = getEnabledLayerNames();
		const auto extensions = getRequiredExtensions();
		const VkInstanceCreateInfo createInfo
		{
			.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
			.pNext = nullptr,
			.flags = {},
			.pApplicationInfo = &applicationInfo,
			.enabledLayerCount = static_cast<std::uint32_t>(enabledLayerNames.size()),
			.ppEnabledLayerNames = enabledLayerNames.data(),
			.enabledExtensionCount = static_cast<std::uint32_t>(extensions.size()),
			.ppEnabledExtensionNames = extensions.data()
		};

		PrintAPIVersion();

		const auto result = vkCreateInstance(&createInfo, nullptr, &get());
		if (result == VK_SUCCESS)
		{
			return true;
		}
		else
		{
			Logger->error("Couldn't create Instance, result: {}", static_cast<std::int32_t>(result));
			return false;
		}
	}

	void Instance::destroy() noexcept
	{
		if (isValid())
		{
			vkDestroyInstance(get(), nullptr);
			invalidateInternal();
		}
	}

	std::vector<const char*> Instance::getEnabledLayerNames() const
	{
		if (enableValidationLayers)
			return { "VK_LAYER_KHRONOS_validation" };
		else
			return {};
	}

	bool Instance::areEnabledLayersSupported() const
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

	std::vector<const char*> Instance::GetGlfwRequiredInstanceExtensions()
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

	std::vector<const char*> Instance::getRequiredExtensions() const
	{
		auto extensions = GetGlfwRequiredInstanceExtensions();

		if (enableValidationLayers)
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

		return extensions;
	}

	std::vector<PhysicalDevice> Instance::getPhysicalDevices() const
	{
		std::uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(get(), &deviceCount, nullptr);

		std::vector<VkPhysicalDevice> rawDevices(deviceCount);
		vkEnumeratePhysicalDevices(get(), &deviceCount, rawDevices.data());

		std::vector<PhysicalDevice> result;
		result.reserve(deviceCount);
		for (auto rawDevice : rawDevices)
		{
			result.emplace_back(rawDevice);
		}
		return result;
	}

}