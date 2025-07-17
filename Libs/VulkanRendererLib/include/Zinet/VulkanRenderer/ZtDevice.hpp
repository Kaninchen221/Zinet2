#pragma once

#include "Zinet/VulkanRenderer/ZtVulkanRendererConfig.hpp"
#include "Zinet/VulkanRenderer/ZtVulkanObject.hpp"
#include "Zinet/VulkanRenderer/ZtQueue.hpp"
#include "Zinet/VulkanRenderer/ZtSwapChain.hpp"

#include "Zinet/Core/ZtLogger.hpp"

#include <vulkan/vulkan.h>

namespace zt::wd
{
	class Window;
}

namespace zt::vulkan_renderer
{
	class Instance;
	class Surface;
	class PhysicalDevice;

	class ZINET_VULKAN_RENDERER_API Device : public VulkanObject<VkDevice>
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("VRDevice");

	public:

		Device(HandleType newObjectHandle)
			: VulkanObject(newObjectHandle)
		{}

		Device() ZINET_API_POST = delete;
		Device(const Device& other) ZINET_API_POST = delete;
		Device(Device&& other) ZINET_API_POST = default;
		~Device() ZINET_API_POST = default;

		Device& operator = (const Device& other) ZINET_API_POST = delete;
		Device& operator = (Device&& other) ZINET_API_POST = default;

		bool create(const Instance& instance, const PhysicalDevice& physicalDevice, const Surface& surface) ZINET_API_POST;

		void destroy() ZINET_API_POST;

		Queue getQueue() ZINET_API_POST;

		const auto& getQueueFamilyIndex() const ZINET_API_POST { return queueFamilyIndex; }

		bool waitIdle() const ZINET_API_POST;

		bool setDebugName(const IsVulkanObjectT auto& vulkanObject, std::string_view debugName, VkObjectType objectType) const ZINET_API_POST;

	protected:

		std::uint32_t queueFamilyIndex = InvalidIndex;

		/// TODO: Refactor this
		PFN_vkSetDebugUtilsObjectNameEXT setDebugUtilsObjectName = nullptr;

	};

	bool Device::setDebugName([[maybe_unused]] const IsVulkanObjectT auto& vulkanObject, [[maybe_unused]] std::string_view debugName, [[maybe_unused]] VkObjectType objectType) const ZINET_API_POST
	{
		/// Should be empty for not debug builds
#		if ZINET_DEBUG
		if (!setDebugUtilsObjectName)
		{
			Logger->error("Ptr to function 'vkSetDebugUtilsObjectNameEXT' is invalid");
			return false;
		}

		const VkDebugUtilsObjectNameInfoEXT debugVulkanObjectName
		{
			.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT,
			.pNext = nullptr,
			.objectType = objectType,
			.objectHandle = uint64_t(vulkanObject.get()),
			.pObjectName = debugName.data()
		};

		const auto result = std::invoke(setDebugUtilsObjectName, get(), &debugVulkanObjectName);
		if (result != VK_SUCCESS)
		{
			Logger->error("Failed registering buffer debug name, result: {}", static_cast<int32_t>(result));
			return false;
		}
#		endif

		return true;
	}
}