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
#include "Zinet/VulkanRenderer/ZtDescriptorSet.hpp"
#include "Zinet/VulkanRenderer/ZtBuffer.hpp"

#include "Zinet/Core/ZtLogger.hpp"

#include <vulkan/vulkan.h>

namespace zt::vulkan_renderer
{
	class RendererContext;
	class DrawInfo;

	class ZINET_VULKAN_RENDERER_API GraphicsPipeline
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("VRGraphicsPipeline");

	public:

		GraphicsPipeline() noexcept = default;
		GraphicsPipeline(const GraphicsPipeline& other) noexcept = delete;
		GraphicsPipeline(GraphicsPipeline&& other) noexcept = default;
		~GraphicsPipeline() noexcept = default;

		GraphicsPipeline& operator = (const GraphicsPipeline& other) noexcept = delete;
		GraphicsPipeline& operator = (GraphicsPipeline&& other) noexcept = default;

		bool create(RendererContext& rendererContext) noexcept;

		void destroy(const RendererContext& rendererContext) noexcept;

		bool preDraw(const RendererContext& rendererContext, const DrawInfo& drawInfo) noexcept;

		void draw(const RendererContext& rendererContext, const DrawInfo& drawInfo) noexcept;

		std::vector<VkImage> images;
		std::vector<ImageView> imageViews;
		std::vector<Framebuffer> framebuffers;

		CommandBuffer commandBuffer{ nullptr };
		Semaphore imageAvailableSemaphore{ nullptr };
		Semaphore renderFinishedSemaphore{ nullptr };
		Fence fence{ nullptr };

		RenderPass renderPass{ nullptr };
		PipelineLayout pipelineLayout{ nullptr };
		Pipeline pipeline{ nullptr };

		DescriptorPool descriptorPool{ nullptr };
		DescriptorSetLayout descriptorSetLayout{ nullptr };
		DescriptorSet descriptorSet{ nullptr };
	};
}