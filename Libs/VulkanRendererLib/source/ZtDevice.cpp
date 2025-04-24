#include "Zinet/VulkanRenderer/ZtDevice.hpp"
#include "Zinet/VulkanRenderer/ZtSurface.hpp"
#include "Zinet/VulkanRenderer/ZtPhysicalDevice.hpp"

#include "Zinet/Core/ZtUtils.hpp"

#include "Zinet/Window/ZtWindow.hpp"

namespace zt::vulkan_renderer
{
	bool Device::create(const PhysicalDevice& physicalDevice, const Surface& surface) noexcept
	{
		if (isValid())
			return false;

		const auto createQueueCreateInfo = [](std::uint32_t queueFamilyIndex, std::uint32_t count, const std::vector<float>& priorities)
		{
			VkDeviceQueueCreateInfo queueCreateInfo{};
			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex = queueFamilyIndex;
			queueCreateInfo.queueCount = count;
			queueCreateInfo.pQueuePriorities = priorities.data();
			return queueCreateInfo;
		};

		auto queuesFamiliesProperties = physicalDevice.getVkQueuesFamiliesProperties();
		physicalDevice.printVkQueuesFamiliesProperties(queuesFamiliesProperties, surface);

		queueFamilyIndex =
			surface.isValid() ? physicalDevice.takeQueueFamilyIndexForPresentAndSurface(queuesFamiliesProperties, surface) : physicalDevice.takeQueueFamilyIndexForPresent(queuesFamiliesProperties);

		if (queueFamilyIndex == InvalidIndex)
		{
			Logger->error("Couldn't get queue family index for present");
			return false;
		}

		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
		std::vector<float> priorities;
		priorities.push_back(1.f);
		queueCreateInfos.push_back(createQueueCreateInfo(queueFamilyIndex, 1, priorities));

		VkPhysicalDeviceFeatures deviceFeatures{};

		VkDeviceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		createInfo.pQueueCreateInfos = queueCreateInfos.data();
		createInfo.queueCreateInfoCount = static_cast<std::uint32_t>(queueCreateInfos.size());
		createInfo.pEnabledFeatures = &deviceFeatures;

		const auto extensions = PhysicalDevice::GetRequiredExtensions();
		createInfo.enabledExtensionCount = static_cast<std::uint32_t>(extensions.size());
		createInfo.ppEnabledExtensionNames = extensions.data();

		// Deprecated and ignored
		createInfo.enabledLayerCount = 0;
		createInfo.ppEnabledLayerNames = nullptr;

		const auto createResult = vkCreateDevice(physicalDevice.get(), &createInfo, nullptr, &objectHandle);

		if (createResult == VK_SUCCESS)
		{
			return true;
		}
		else
		{
			Logger->error("Couldn't create device, result: {}", static_cast<std::int32_t>(createResult));
			return false;
		}
	}

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
		if (queueFamilyIndex == InvalidIndex)
			return Queue{ nullptr, InvalidIndex };

		VkQueue queueObjectHandle = nullptr;
		vkGetDeviceQueue(objectHandle, queueFamilyIndex, 0, &queueObjectHandle);

		return Queue(queueObjectHandle, queueFamilyIndex);
	}
}