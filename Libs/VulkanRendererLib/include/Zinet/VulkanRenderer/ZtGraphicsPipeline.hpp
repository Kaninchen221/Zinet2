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

		using VkDescriptorSets = std::vector<VkDescriptorSet>;

		GraphicsPipeline() noexcept = default;
		GraphicsPipeline(const GraphicsPipeline& other) noexcept = delete;
		GraphicsPipeline(GraphicsPipeline&& other) noexcept = default;
		~GraphicsPipeline() noexcept = default;

		GraphicsPipeline& operator = (const GraphicsPipeline& other) noexcept = delete;
		GraphicsPipeline& operator = (GraphicsPipeline&& other) noexcept = default;

		bool create(const RendererContext& rendererContext, DrawInfo& drawInfo);

		void destroy(const RendererContext& rendererContext) noexcept;

		void draw(const RendererContext& rendererContext, const DrawInfo& drawInfo);

		bool submit(const RendererContext& rendererContext);

		bool isValid() const noexcept;

		CommandBuffer commandBuffer{ nullptr };

		PipelineLayout pipelineLayout{ nullptr };
		Pipeline pipeline{ nullptr };

		// Descriptors
		DescriptorPool descriptorPool{ nullptr };

		DescriptorSetLayout pipelineDescriptorSetLayout{ nullptr };
		DescriptorSets pipelineDescriptorSet{ nullptr };

		DescriptorSetLayout objectDescriptorSetLayout{ nullptr };
		DescriptorSets objectDescriptorSet{ nullptr };

	protected:

		using DescriptorPoolSizes = std::vector<VkDescriptorPoolSize>;
		void createDescriptorData(
			DescriptorSetLayout::Bindings& outBindings, 
			DescriptorPoolSizes& outDescriptorPoolSizes, 
			DescriptorInfo& descriptorInfo) const;

		DescriptorSetLayout createDescriptorSetLayout(const Device& device, DescriptorSetLayout::Bindings& bindings);

		DescriptorSets createDescriptorSet(
			const Device& device,
			const DescriptorSetLayout& layout, 
			std::vector<VkDescriptorSetLayout>& outLayouts);

		static void UpdateDescriptorSet(const Device& device, const DescriptorInfo& descriptorInfo, const DescriptorSets& descriptorSet);

		struct Data {
			VkDescriptorSets vkDescriptorSets;
		};
		Data data;

	};
}