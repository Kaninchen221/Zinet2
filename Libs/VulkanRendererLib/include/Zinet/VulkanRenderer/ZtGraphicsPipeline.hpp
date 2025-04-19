#pragma once

#include "Zinet/VulkanRenderer/ZtVulkanRendererConfig.hpp"
#include "Zinet/VulkanRenderer/ZtVulkanObject.hpp"

#include "Zinet/Core/ZtLogger.hpp"

#include <vulkan/vulkan.h>

namespace zt::vulkan_renderer
{
	class Device;
	class PipelineLayout;
	class RenderPass;

	class ZINET_VULKAN_RENDERER_API GraphicsPipeline : public VulkanObject<VkPipeline>
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("VRGraphicsPipeline");

	public:

		GraphicsPipeline(HandleType newObjectHandle)
			: VulkanObject(newObjectHandle)
		{}

		GraphicsPipeline() noexcept = delete;
		GraphicsPipeline(const GraphicsPipeline& other) noexcept = delete;
		GraphicsPipeline(GraphicsPipeline&& other) noexcept = default;
		~GraphicsPipeline() noexcept = default;

		GraphicsPipeline& operator = (const GraphicsPipeline& other) noexcept = delete;
		GraphicsPipeline& operator = (GraphicsPipeline&& other) noexcept = default;

		using ShadersStages = std::vector<VkPipelineShaderStageCreateInfo>;

		bool create(
			const Device& device, 
			const PipelineLayout& pipelineLayout, 
			const RenderPass& renderPass, 
			const VkViewport& viewport, 
			const VkRect2D& scissor,
			const ShadersStages& shadersStages) noexcept;

		void destroy(const Device& device) noexcept;

		VkPipelineDynamicStateCreateInfo createVkPipelineDynamicStateCreateInfo(
			const std::vector<VkDynamicState>& dynamicStates) const noexcept;

		VkPipelineVertexInputStateCreateInfo createVkPipelineVertexInputStateCreateInfo() const noexcept;

		VkPipelineInputAssemblyStateCreateInfo createVkPipelineInputAssemblyStateCreateInfo() const noexcept;

		VkPipelineViewportStateCreateInfo createVkPipelineViewportStateCreateInfo(
			const VkViewport& viewport, const VkRect2D& scissor) const noexcept;

		VkPipelineRasterizationStateCreateInfo createVkPipelineRasterizationStateCreateInfo() const noexcept;

		VkPipelineMultisampleStateCreateInfo createVkPipelineMultisampleStateCreateInfo() const noexcept;

		VkPipelineColorBlendAttachmentState createVkPipelineColorBlendAttachmentState() const noexcept;

		VkPipelineColorBlendStateCreateInfo createVkPipelineColorBlendStateCreateInfo(
			const VkPipelineColorBlendAttachmentState& colorBlendAttachmentState) const noexcept;
	};
}