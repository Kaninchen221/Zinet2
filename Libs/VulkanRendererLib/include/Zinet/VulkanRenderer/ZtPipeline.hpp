#pragma once

#include "Zinet/VulkanRenderer/ZtVulkanRendererConfig.hpp"
#include "Zinet/VulkanRenderer/ZtVulkanObject.hpp"
#include "Zinet/VulkanRenderer/ZtVertex.hpp"

#include "Zinet/Core/ZtLogger.hpp"

#include <vulkan/vulkan.h>

namespace zt::vulkan_renderer
{
	class Device;
	class PipelineLayout;
	class RenderPass;
	class DrawInfo;

	class ZINET_VULKAN_RENDERER_API Pipeline : public VulkanObject<VkPipeline>
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("VRPipeline");

	public:

		Pipeline(HandleType newObjectHandle)
			: VulkanObject(newObjectHandle)
		{}

		Pipeline() noexcept = delete;
		Pipeline(const Pipeline& other) noexcept = delete;
		Pipeline(Pipeline&& other) noexcept = default;
		~Pipeline() noexcept = default;

		Pipeline& operator = (const Pipeline& other) noexcept = delete;
		Pipeline& operator = (Pipeline&& other) noexcept = default;

		using ShadersStages = std::vector<VkPipelineShaderStageCreateInfo>;

		bool create(
			const Device& device, 
			const PipelineLayout& pipelineLayout, 
			const RenderPass& renderPass, 
			const VkViewport& viewport, 
			const VkRect2D& scissor, 
			const DrawInfo& drawInfo) noexcept;

		void destroy(const Device& device) noexcept;

		Pipeline::ShadersStages createShadersStages(const DrawInfo& drawInfo) const noexcept;

		VkPipelineDynamicStateCreateInfo createVkPipelineDynamicStateCreateInfo(
			const std::vector<VkDynamicState>& dynamicStates) const noexcept;

		VkPipelineDynamicStateCreateInfo createVkPipelineDynamicStateCreateInfo(
			std::vector<VkDynamicState>&& dynamicStates) const noexcept = delete;

		VkPipelineVertexInputStateCreateInfo createVkPipelineVertexInputStateCreateInfo(
			const Vertex::InputBindingDescription* bindingDescription,
			const Vertex::InputAttributesDescriptions* attributesDescriptions) const noexcept;

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