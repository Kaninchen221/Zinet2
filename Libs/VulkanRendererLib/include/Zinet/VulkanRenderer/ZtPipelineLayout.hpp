#pragma once

#include "Zinet/VulkanRenderer/ZtVulkanRendererConfig.hpp"
#include "Zinet/VulkanRenderer/ZtVulkanObject.hpp"
#include "Zinet/VulkanRenderer/ZtDescriptorSetLayout.hpp"

#include "Zinet/Core/ZtLogger.hpp"

#include <vulkan/vulkan.h>

namespace zt::vulkan_renderer
{
	class Device;

	class ZINET_VULKAN_RENDERER_API PipelineLayout : public VulkanObject<VkPipelineLayout>
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("VRPipelineLayout");

	public:

		PipelineLayout(HandleType newObjectHandle)
			: VulkanObject(newObjectHandle) {}

		PipelineLayout() ZINET_API_POST = delete;
		PipelineLayout(const PipelineLayout& other) ZINET_API_POST = delete;
		PipelineLayout(PipelineLayout&& other) ZINET_API_POST = default;
		~PipelineLayout() ZINET_API_POST = default;

		PipelineLayout& operator = (const PipelineLayout& other) ZINET_API_POST = delete;
		PipelineLayout& operator = (PipelineLayout&& other) ZINET_API_POST = default;
	
		static VkPipelineLayoutCreateInfo GetDefaultCreateInfo(const std::vector<DescriptorSetLayout::HandleType>& vkDescriptorSetLayouts) ZINET_API_POST;

		bool create(const Device& device, const VkPipelineLayoutCreateInfo& createInfo) ZINET_API_POST;

		void destroy(const Device& device) ZINET_API_POST;

	};
}