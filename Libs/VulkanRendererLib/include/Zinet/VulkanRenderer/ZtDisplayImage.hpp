#pragma once

#include "Zinet/VulkanRenderer/ZtVulkanRendererConfig.hpp"
#include "Zinet/VulkanRenderer/ZtImageView.hpp"
#include "Zinet/VulkanRenderer/ZtFramebuffer.hpp"
#include "Zinet/VulkanRenderer/ZtFence.hpp"
#include "Zinet/VulkanRenderer/ZtCommandBuffer.hpp"
#include "Zinet/VulkanRenderer/ZtDescriptorPool.hpp"
#include "Zinet/VulkanRenderer/ZtDescriptorSetLayout.hpp"
#include "Zinet/VulkanRenderer/ZtDescriptorSets.hpp"
#include "Zinet/VulkanRenderer/ZtDrawInfo.hpp"

#include "Zinet/Core/ZtLogger.hpp"

#include <vulkan/vulkan.h>

namespace zt::vulkan_renderer
{
	class RendererContext;

	class DisplayImage
	{
		inline static auto Logger = core::ConsoleLogger::Create("zt::vulkan_renderer::DisplayImage");

	public:
		using DescriptorPoolSizes = std::vector<VkDescriptorPoolSize>;
		using VkDescriptorSets = std::vector<VkDescriptorSet>;

		VkImage image{};
		ImageView imageView{ nullptr };
		Framebuffer framebuffer{ nullptr };
		Fence fence{ nullptr };
		CommandBuffer commandBuffer{ nullptr };

		DescriptorPool descriptorPool{ nullptr };

		DescriptorSetLayout pipelineDescriptorSetLayout{ nullptr };
		DescriptorSets pipelineDescriptorSet{ nullptr };

		DescriptorSetLayout objectDescriptorSetLayout{ nullptr };
		DescriptorSets objectDescriptorSet{ nullptr };

		VkDescriptorSets vkDescriptorSets;

		bool create(RendererContext& rendererContext, VkImage swapChainImage);

		void destroy(RendererContext& rendererContext);

		static void UpdateDescriptorSet(const Device& device, const DescriptorInfo& descriptorInfo, const DescriptorSets& descriptorSet);

	protected:

		void createDescriptorData(
			DescriptorSetLayout::Bindings& outBindings,
			DescriptorInfo& descriptorInfo) const;

		DescriptorSetLayout createDescriptorSetLayout(const Device& device, DescriptorSetLayout::Bindings& bindings);

		DescriptorSets createDescriptorSet(
			const Device& device,
			const DescriptorSetLayout& layout,
			std::vector<VkDescriptorSetLayout>& outLayouts);



	};
}