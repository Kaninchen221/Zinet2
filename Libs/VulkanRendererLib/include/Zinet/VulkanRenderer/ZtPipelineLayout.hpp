#pragma once

#include "Zinet/VulkanRenderer/ZtVulkanRendererConfig.hpp"
#include "Zinet/VulkanRenderer/ZtVulkanObject.hpp"
#include "Zinet/VulkanRenderer/ZtDescriptorSetLayout.hpp"

#include "Zinet/Core/ZtLogger.hpp"

#include <vulkan/vulkan.h>

namespace zt::vulkan_renderer
{
	class Device;

	class  PipelineLayout : public VulkanObject<VkPipelineLayout>
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("VRPipelineLayout");

	public:

		PipelineLayout(VulcanType newHandle)
			: VulkanObject(newHandle) {}

		PipelineLayout() noexcept = delete;
		PipelineLayout(const PipelineLayout& other) noexcept = delete;
		PipelineLayout(PipelineLayout&& other) noexcept = default;
		~PipelineLayout() noexcept = default;

		PipelineLayout& operator = (const PipelineLayout& other) noexcept = delete;
		PipelineLayout& operator = (PipelineLayout&& other) noexcept = default;
	
		static VkPipelineLayoutCreateInfo GetDefaultCreateInfo(const std::vector<DescriptorSetLayout::VulcanType>& vkDescriptorSetLayouts) noexcept;

		bool create(const Device& device, const VkPipelineLayoutCreateInfo& createInfo);

		void destroy(const Device& device) noexcept;

	};
}