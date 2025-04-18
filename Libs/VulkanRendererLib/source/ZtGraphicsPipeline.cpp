#include "Zinet/VulkanRenderer/ZtGraphicsPipeline.hpp"

namespace zt::vulkan_renderer
{

	VkPipelineDynamicStateCreateInfo GraphicsPipeline::createVkPipelineDynamicStateCreateInfo(
		const std::vector<VkDynamicState>& dynamicStates) const noexcept
	{
		VkPipelineDynamicStateCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		createInfo.dynamicStateCount = static_cast<std::uint32_t>(dynamicStates.size());
		createInfo.pDynamicStates = dynamicStates.data();

		return createInfo;
	}

	VkPipelineVertexInputStateCreateInfo GraphicsPipeline::createVkPipelineVertexInputStateCreateInfo() const noexcept
	{
		VkPipelineVertexInputStateCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		createInfo.vertexBindingDescriptionCount = 0;
		createInfo.pVertexBindingDescriptions = nullptr; // Optional
		createInfo.vertexAttributeDescriptionCount = 0;
		createInfo.pVertexAttributeDescriptions = nullptr; // Optional

		return createInfo;
	}

	VkPipelineInputAssemblyStateCreateInfo GraphicsPipeline::createVkPipelineInputAssemblyStateCreateInfo() const noexcept
	{
		VkPipelineInputAssemblyStateCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		createInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		createInfo.primitiveRestartEnable = VK_FALSE;

		return createInfo;
	}

	VkPipelineViewportStateCreateInfo GraphicsPipeline::createVkPipelineViewportStateCreateInfo(
		const VkViewport& viewport, const VkRect2D& scissor) const noexcept
	{
		VkPipelineViewportStateCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		createInfo.viewportCount = 1;
		createInfo.pViewports = &viewport;
		createInfo.scissorCount = 1;
		createInfo.pScissors = &scissor;

		return createInfo;
	}

	VkPipelineRasterizationStateCreateInfo GraphicsPipeline::createVkPipelineRasterizationStateCreateInfo() const noexcept
	{
		VkPipelineRasterizationStateCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		createInfo.depthClampEnable = VK_FALSE;
		createInfo.rasterizerDiscardEnable = VK_FALSE;
		createInfo.polygonMode = VK_POLYGON_MODE_FILL;
		createInfo.lineWidth = 1.0f;
		createInfo.cullMode = VK_CULL_MODE_NONE; // VK_CULL_MODE_BACK_BIT;
		createInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
		createInfo.depthBiasEnable = VK_FALSE;
		createInfo.depthBiasConstantFactor = 0.0f;
		createInfo.depthBiasClamp = 0.0f;
		createInfo.depthBiasSlopeFactor = 0.0f;

		return createInfo;
	}

	VkPipelineMultisampleStateCreateInfo GraphicsPipeline::createVkPipelineMultisampleStateCreateInfo() const noexcept
	{
		VkPipelineMultisampleStateCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		createInfo.sampleShadingEnable = VK_FALSE;
		createInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		createInfo.minSampleShading = 1.0f;
		createInfo.pSampleMask = nullptr;
		createInfo.alphaToCoverageEnable = VK_FALSE;
		createInfo.alphaToOneEnable = VK_FALSE;

		return createInfo;
	}

	VkPipelineColorBlendAttachmentState GraphicsPipeline::createVkPipelineColorBlendAttachmentState() const noexcept
	{
		VkPipelineColorBlendAttachmentState attachmentState{};
		attachmentState.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		attachmentState.blendEnable = VK_TRUE;
		attachmentState.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
		attachmentState.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
		attachmentState.colorBlendOp = VK_BLEND_OP_ADD;
		attachmentState.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
		attachmentState.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
		attachmentState.alphaBlendOp = VK_BLEND_OP_ADD;

		return attachmentState;
	}

	VkPipelineColorBlendStateCreateInfo GraphicsPipeline::createVkPipelineColorBlendStateCreateInfo(
		const VkPipelineColorBlendAttachmentState& colorBlendAttachmentState) const noexcept
	{
		VkPipelineColorBlendStateCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		createInfo.logicOpEnable = VK_TRUE;
		createInfo.logicOp = VK_LOGIC_OP_COPY;
		createInfo.attachmentCount = 1;
		createInfo.pAttachments = &colorBlendAttachmentState;
		createInfo.blendConstants[0] = 0.0f;
		createInfo.blendConstants[1] = 0.0f;
		createInfo.blendConstants[2] = 0.0f;
		createInfo.blendConstants[3] = 0.0f;

		return createInfo;
	}

}