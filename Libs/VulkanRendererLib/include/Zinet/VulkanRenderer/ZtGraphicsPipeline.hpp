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
#include "Zinet/VulkanRenderer/ZtBuffer.hpp"
#include "Zinet/VulkanRenderer/ZtDescriptorSets.hpp"

#include "Zinet/Core/ZtLogger.hpp"

#include <vulkan/vulkan.h>

namespace zt::vulkan_renderer
{
	class RendererContext;
	struct DrawInfo;
	struct DescriptorInfo;

	struct GraphicsPipelineCreateInfo
	{
		const RendererContext& rendererContext;
		DrawInfo& drawInfo;
		size_t descriptorSetsCount = 1;
	};

	class  GraphicsPipeline
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("zt::vulkan_renderer::GraphicsPipeline");

	public:

		GraphicsPipeline() noexcept = default;
		GraphicsPipeline(const GraphicsPipeline& other) noexcept = delete;
		GraphicsPipeline(GraphicsPipeline&& other) noexcept = default;
		~GraphicsPipeline() noexcept = default;

		GraphicsPipeline& operator = (const GraphicsPipeline& other) noexcept = delete;
		GraphicsPipeline& operator = (GraphicsPipeline&& other) noexcept = default;

		bool create(const GraphicsPipelineCreateInfo& createInfo);

		void destroy(const RendererContext& rendererContext) noexcept;

		void draw(RendererContext& rendererContext, const DrawInfo& drawInfo, CommandBuffer& commandBuffer);

		bool isValid() const noexcept;

		auto& getPipeline() noexcept { return pipeline; }
		auto& getPipeline() const noexcept { return pipeline; }

		auto& getPipelineLayout() noexcept { return pipelineLayout; }
		auto& getPipelineLayout() const noexcept { return pipelineLayout; }

	protected:

		DescriptorSetLayout pipelineDescriptorSetLayout{ nullptr };
		DescriptorSets pipelineDescriptorSets;

		DescriptorSetLayout objectDescriptorSetLayout{ nullptr };
		DescriptorSets objectDescriptorSets;

		using VkDescriptorSets = std::array<VkDescriptorSet, 2>;
		VkDescriptorSets vkDescriptorSets;

		PipelineLayout pipelineLayout{ nullptr };
		Pipeline pipeline{ nullptr };

		bool createDescriptors(const GraphicsPipelineCreateInfo& graphicsPipelineCreateInfo);

	};
}