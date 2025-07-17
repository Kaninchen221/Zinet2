#include "Zinet/VulkanRenderer/ZtDevice.hpp"
#include "Zinet/VulkanRenderer/ZtSurface.hpp"
#include "Zinet/VulkanRenderer/ZtPhysicalDevice.hpp"
#include "Zinet/VulkanRenderer/ZtInstance.hpp"

#include "Zinet/Core/ZtUtils.hpp"

#include "Zinet/Window/ZtWindow.hpp"

namespace zt::vulkan_renderer
{
	bool Device::create(const Instance& instance, const PhysicalDevice& physicalDevice, const Surface& surface) ZINET_API_POST
	{
		if (isValid())
			return false;

		setDebugUtilsObjectName = reinterpret_cast<PFN_vkSetDebugUtilsObjectNameEXT>(vkGetInstanceProcAddr(instance.get(), "vkSetDebugUtilsObjectNameEXT"));

		const auto createQueueCreateInfo = [](std::uint32_t queueFamilyIndex, std::uint32_t count, const std::vector<float>& priorities)
		{
			return VkDeviceQueueCreateInfo
			{
				.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
				.pNext = nullptr,
				.flags = {},
				.queueFamilyIndex = queueFamilyIndex,
				.queueCount = count,
				.pQueuePriorities = priorities.data()
			};
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

		const VkPhysicalDeviceFeatures deviceFeatures{};

		const auto extensions = PhysicalDevice::GetRequiredExtensions();

		/// We need it for: "#extension GL_EXT_nonuniform_qualifier : require" GLSL extension
		VkPhysicalDeviceDescriptorIndexingFeaturesEXT indexingFeatures = {};
		indexingFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_FEATURES_EXT;
		indexingFeatures.runtimeDescriptorArray = VK_TRUE;
		indexingFeatures.shaderSampledImageArrayNonUniformIndexing = VK_TRUE;
		indexingFeatures.descriptorBindingVariableDescriptorCount = VK_TRUE;

		const VkDeviceCreateInfo createInfo
		{
			.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
			.pNext = &indexingFeatures,
			.flags = {},
			.queueCreateInfoCount = static_cast<std::uint32_t>(queueCreateInfos.size()),
			.pQueueCreateInfos = queueCreateInfos.data(),
			.enabledLayerCount = 0,
			.ppEnabledLayerNames = nullptr,
			.enabledExtensionCount = static_cast<std::uint32_t>(extensions.size()),
			.ppEnabledExtensionNames = extensions.data(),
			.pEnabledFeatures = &deviceFeatures
		};

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

	void Device::destroy() ZINET_API_POST
	{
		if (isValid())
		{
			vkDestroyDevice(objectHandle, nullptr);
			objectHandle = nullptr;
		}
	}

	Queue Device::getQueue() ZINET_API_POST
	{
		if (queueFamilyIndex == InvalidIndex)
			return Queue{ nullptr, InvalidIndex };

		VkQueue queueObjectHandle = nullptr;
		vkGetDeviceQueue(objectHandle, queueFamilyIndex, 0, &queueObjectHandle);

		return Queue(queueObjectHandle, queueFamilyIndex);
	}

	bool Device::waitIdle() const ZINET_API_POST
	{
		const auto result = vkDeviceWaitIdle(objectHandle);
		if (result == VK_SUCCESS)
		{
			return true;
		}
		else
		{
			Logger->error("Device can't wait idle, result: {}", static_cast<std::int32_t>(result));
			return false;
		}
	}

}