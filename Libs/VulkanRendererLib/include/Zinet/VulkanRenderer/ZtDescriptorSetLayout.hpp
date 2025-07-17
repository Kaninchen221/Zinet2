#pragma once

#include "Zinet/VulkanRenderer/ZtVulkanRendererConfig.hpp"
#include "Zinet/VulkanRenderer/ZtVulkanObject.hpp"
#include "Zinet/VulkanRenderer/ZtPhysicalDevice.hpp"
#include "Zinet/VulkanRenderer/ZtSurface.hpp"

#include "Zinet/Core/ZtLogger.hpp"

#include <vulkan/vulkan.h>

namespace zt::vulkan_renderer
{
	class Device;

	class ZINET_VULKAN_RENDERER_API DescriptorSetLayout : public VulkanObject<VkDescriptorSetLayout>
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("VRDescriptorSetLayout");

	public:

		DescriptorSetLayout(HandleType newObjectHandle)
			: VulkanObject(newObjectHandle) {}

		DescriptorSetLayout() ZINET_API_POST = delete;
		DescriptorSetLayout(const DescriptorSetLayout& other) ZINET_API_POST = delete;
		DescriptorSetLayout(DescriptorSetLayout&& other) ZINET_API_POST = default;
		~DescriptorSetLayout() ZINET_API_POST = default;

		DescriptorSetLayout& operator = (const DescriptorSetLayout& other) ZINET_API_POST = delete;
		DescriptorSetLayout& operator = (DescriptorSetLayout&& other) ZINET_API_POST = default;

		static VkDescriptorSetLayoutBinding GetDefaultUniformLayoutBinding() ZINET_API_POST;
		static VkDescriptorSetLayoutBinding GetDefaultImageLayoutBinding() ZINET_API_POST;

		using Bindings = std::vector<VkDescriptorSetLayoutBinding>;
		static VkDescriptorSetLayoutCreateInfo GetDefaultCreateInfo(const Bindings& bindings) ZINET_API_POST;
		static VkDescriptorSetLayoutCreateInfo GetDefaultCreateInfo(Bindings&& bindings) ZINET_API_POST = delete;

		bool create(const VkDescriptorSetLayoutCreateInfo& createInfo, const Device& device) ZINET_API_POST;

		void destroy(const Device& device) ZINET_API_POST;

	};
}