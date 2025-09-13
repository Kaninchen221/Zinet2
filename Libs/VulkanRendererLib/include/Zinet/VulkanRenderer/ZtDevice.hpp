#pragma once

#include "Zinet/VulkanRenderer/ZtVulkanRendererConfig.hpp"
#include "Zinet/VulkanRenderer/ZtVulkanObject.hpp"
#include "Zinet/VulkanRenderer/ZtQueue.hpp"
#include "Zinet/VulkanRenderer/ZtSwapChain.hpp"

#include "Zinet/Core/ZtLogger.hpp"
#include "Zinet/Core/ZtFunction.hpp"

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

	class  Device : public VulkanObject<VkDevice>
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("VRDevice");

	public:

		Device(VulcanType newHandle) noexcept
			: VulkanObject(newHandle)
		{}

		Device() noexcept = delete;
		Device(const Device& other) noexcept = delete;
		Device(Device&& other) noexcept = default;
		~Device() noexcept = default;

		Device& operator = (const Device& other) noexcept = delete;
		Device& operator = (Device&& other) noexcept = default;

		bool create(const Instance& instance, const PhysicalDevice& physicalDevice, const Surface& surface);

		void destroy() noexcept;

		Queue getQueue() noexcept;

		const auto& getQueueFamilyIndex() const noexcept { return queueFamilyIndex; }

		bool waitIdle() const;

		bool setDebugName(const auto& vulkanObject, std::string_view debugName, VkObjectType objectType) const;

	protected:

		std::uint32_t queueFamilyIndex = InvalidIndex;

		using SetDebugUtilsObjectNameT = core::Function<VkResult, VkDevice, const VkDebugUtilsObjectNameInfoEXT*>;
		SetDebugUtilsObjectNameT setDebugUtilsObjectName;

	};

	bool Device::setDebugName(
		[[maybe_unused]] const auto& vulkanObject, [[maybe_unused]] std::string_view debugName, [[maybe_unused]] VkObjectType objectType) const
	{
		/// Should be empty for not debug builds
#		if ZINET_DEBUG
		if (!setDebugUtilsObjectName)
		{
			Logger->info("Ptr to function 'vkSetDebugUtilsObjectNameEXT' is invalid, most probably because validation layers are turned off");
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

		const auto result = setDebugUtilsObjectName.invoke(get(), &debugVulkanObjectName);
		if (result != VK_SUCCESS)
		{
			Logger->error("Failed registering buffer debug name, result: {}", static_cast<int32_t>(result));
			return false;
		}
#		endif

		return true;
	}
}