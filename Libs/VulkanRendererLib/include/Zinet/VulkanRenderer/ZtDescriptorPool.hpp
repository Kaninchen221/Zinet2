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

		DescriptorPool() ZINET_API_POST = delete;
		DescriptorPool(const DescriptorPool& other) ZINET_API_POST = delete;
		DescriptorPool(DescriptorPool&& other) ZINET_API_POST = default;
		~DescriptorPool() ZINET_API_POST = default;

		DescriptorPool& operator = (const DescriptorPool& other) ZINET_API_POST = delete;
		DescriptorPool& operator = (DescriptorPool&& other) ZINET_API_POST = default;

		static VkDescriptorPoolSize GetDefaultDescriptorPoolSize() ZINET_API_POST;

		static VkDescriptorPoolCreateInfo GetDefaultCreateInfo(const std::vector<VkDescriptorPoolSize>& poolSizes) ZINET_API_POST;
		static VkDescriptorPoolCreateInfo GetDefaultCreateInfo(const std::vector<VkDescriptorPoolSize>&& poolSizes) ZINET_API_POST = delete;

		bool create(const Device& device, const VkDescriptorPoolCreateInfo& createInfo) ZINET_API_POST;

		void destroy(const Device& device) ZINET_API_POST;

	};
}