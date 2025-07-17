#pragma once

#include "Zinet/VulkanRenderer/ZtVulkanRendererConfig.hpp"
#include "Zinet/VulkanRenderer/ZtImageView.hpp"
#include "Zinet/VulkanRenderer/ZtFramebuffer.hpp"
#include "Zinet/VulkanRenderer/ZtCommandBuffer.hpp"
#include "Zinet/VulkanRenderer/ZtSemaphore.hpp"
#include "Zinet/VulkanRenderer/ZtFence.hpp"
#include "Zinet/VulkanRenderer/ZtRenderPass.hpp"
#include "Zinet/VulkanRenderer/ZtPipelineLayout.hpp"
#include "Zinet/VulkanRenderer/ZtPipeline.hpp"

#include "Zinet/VulkanRenderer/ZtDescriptorPool.hpp"
#include "Zinet/VulkanRenderer/ZtDescriptorSetLayout.hpp"
#include "Zinet/VulkanRenderer/ZtDescriptorSets.hpp"
#include "Zinet/VulkanRenderer/ZtBuffer.hpp"

#include "Zinet/Core/ZtLogger.hpp"

#include <vulkan/vulkan.h>

namespace zt::vulkan_renderer
{
	class RendererContext;
	struct DrawInfo;
	struct DescriptorInfo;

	class ZINET_VULKAN_RENDERER_API GraphicsPipeline
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("VRGraphicsPipeline");

	public:

		GraphicsPipeline() ZINET_API_POST = default;
		GraphicsPipeline(const GraphicsPipeline& other) ZINET_API_POST = delete;
		GraphicsPipeline(GraphicsPipeline&& other) ZINET_API_POST = default;
		~GraphicsPipeline() ZINET_API_POST = default;

		GraphicsPipeline& operator = (const GraphicsPipeline& other) ZINET_API_POST = delete;
		GraphicsPipeline& operator = (GraphicsPipeline&& other) ZINET_API_POST = default;

		bool create(const RendererContext& rendererContext, DrawInfo& drawInfo) ZINET_API_POST;

		void destroy(const RendererContext& rendererContext) ZINET_API_POST;

		void draw(const RendererContext& rendererContext, const DrawInfo& drawInfo) ZINET_API_POST;

		bool submit(const RendererContext& rendererContext) ZINET_API_POST;

		bool isValid() const ZINET_API_POST;

		CommandBuffer commandBuffer{ nullptr };

		PipelineLayout pipelineLayout{ nullptr };
		Pipeline pipeline{ nullptr };

		// Descriptors
		DescriptorPool descriptorPool{ nullptr };

		DescriptorSetLayout pipelineDescriptorSetLayout{ nullptr };
		DescriptorSets pipelineDescriptorSet{ nullptr };

		DescriptorSetLayout objectDescriptorSetLayout{ nullptr };
		DescriptorSets objectDescriptorSet{ nullptr };

		using VkDescriptorSets = std::vector<VkDescriptorSet>;
		VkDescriptorSets vkDescriptorSets;

	protected:

		using DescriptorPoolSizes = std::vector<VkDescriptorPoolSize>;
		void createDescriptorData(
			DescriptorSetLayout::Bindings& outBindings, 
			DescriptorPoolSizes& outDescriptorPoolSizes, 
			DescriptorInfo& descriptorInfo) const ZINET_API_POST;

		DescriptorSetLayout createDescriptorSetLayout(const Device& device, DescriptorSetLayout::Bindings& bindings) ZINET_API_POST;

		DescriptorSets createDescriptorSet(
			const Device& device,
			const DescriptorSetLayout& layout, 
			std::vector<VkDescriptorSetLayout>& outLayouts) ZINET_API_POST;

		static void UpdateDescriptorSet(const Device& device, const DescriptorInfo& descriptorInfo, const DescriptorSets& descriptorSet) ZINET_API_POST;

	};
}