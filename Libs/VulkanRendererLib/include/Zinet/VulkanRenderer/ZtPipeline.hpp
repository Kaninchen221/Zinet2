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
	struct DrawInfo;

	class ZINET_VULKAN_RENDERER_API Pipeline : public VulkanObject<VkPipeline>
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("VRPipeline");

	public:

		Pipeline(HandleType newObjectHandle)
			: VulkanObject(newObjectHandle)
		{}

		Pipeline() ZINET_API_POST = delete;
		Pipeline(const Pipeline& other) ZINET_API_POST = delete;
		Pipeline(Pipeline&& other) ZINET_API_POST = default;
		~Pipeline() ZINET_API_POST = default;

		Pipeline& operator = (const Pipeline& other) ZINET_API_POST = delete;
		Pipeline& operator = (Pipeline&& other) ZINET_API_POST = default;

		using ShadersStages = std::vector<VkPipelineShaderStageCreateInfo>;

		bool create(
			const Device& device, 
			const PipelineLayout& pipelineLayout, 
			const RenderPass& renderPass, 
			const VkViewport& viewport, 
			const VkRect2D& scissor, 
			const DrawInfo& drawInfo) ZINET_API_POST;

		void destroy(const Device& device) ZINET_API_POST;

		Pipeline::ShadersStages createShadersStages(const DrawInfo& drawInfo) const ZINET_API_POST;

		VkPipelineDynamicStateCreateInfo createVkPipelineDynamicStateCreateInfo(
			const std::vector<VkDynamicState>& dynamicStates) const ZINET_API_POST;

		VkPipelineDynamicStateCreateInfo createVkPipelineDynamicStateCreateInfo(
			std::vector<VkDynamicState>&& dynamicStates) const ZINET_API_POST = delete;

		VkPipelineVertexInputStateCreateInfo createVkPipelineVertexInputStateCreateInfo(
			const Vertex::InputBindingDescription* bindingDescription,
			const Vertex::InputAttributesDescriptions* attributesDescriptions) const ZINET_API_POST;

		VkPipelineInputAssemblyStateCreateInfo createVkPipelineInputAssemblyStateCreateInfo() const ZINET_API_POST;

		VkPipelineViewportStateCreateInfo createVkPipelineViewportStateCreateInfo(
			const VkViewport& viewport, const VkRect2D& scissor) const ZINET_API_POST;

		VkPipelineRasterizationStateCreateInfo createVkPipelineRasterizationStateCreateInfo() const ZINET_API_POST;

		VkPipelineMultisampleStateCreateInfo createVkPipelineMultisampleStateCreateInfo() const ZINET_API_POST;

		VkPipelineColorBlendAttachmentState createVkPipelineColorBlendAttachmentState() const ZINET_API_POST;

		VkPipelineColorBlendStateCreateInfo createVkPipelineColorBlendStateCreateInfo(
			const VkPipelineColorBlendAttachmentState& colorBlendAttachmentState) const ZINET_API_POST;
	};
}