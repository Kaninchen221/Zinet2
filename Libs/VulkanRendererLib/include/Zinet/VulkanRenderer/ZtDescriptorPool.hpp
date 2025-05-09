#pragma once

#include "Zinet/VulkanRenderer/ZtVulkanRendererConfig.hpp"
#include "Zinet/VulkanRenderer/ZtVulkanObject.hpp"

#include "Zinet/Core/ZtLogger.hpp"

#include <vulkan/vulkan.h>

namespace zt::vulkan_renderer
{
	class Device;

	class ZINET_VULKAN_RENDERER_API DescriptorPool : public VulkanObject<VkDescriptorPool>
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("VRDescriptorPool");

	public:

		DescriptorPool(HandleType newObjectHandle)
			: VulkanObject(newObjectHandle) {}

		DescriptorPool() noexcept = delete;
		DescriptorPool(const DescriptorPool& other) noexcept = delete;
		DescriptorPool(DescriptorPool&& other) noexcept = default;
		~DescriptorPool() noexcept = default;

		DescriptorPool& operator = (const DescriptorPool& other) noexcept = delete;
		DescriptorPool& operator = (DescriptorPool&& other) noexcept = default;

		static VkDescriptorPoolSize GetDefaultDescriptorPoolSize() noexcept;

		static VkDescriptorPoolCreateInfo GetDefaultCreateInfo(const std::vector<VkDescriptorPoolSize>& poolSizes) noexcept;
		static VkDescriptorPoolCreateInfo GetDefaultCreateInfo(const std::vector<VkDescriptorPoolSize>&& poolSizes) noexcept = delete;

		bool create(const Device& device, const VkDescriptorPoolCreateInfo& createInfo) noexcept;

		void destroy(const Device& device) noexcept;

	};
}