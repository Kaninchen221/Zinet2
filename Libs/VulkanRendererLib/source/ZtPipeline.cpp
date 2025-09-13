#include "Zinet/VulkanRenderer/ZtPipeline.hpp"
#include "Zinet/VulkanRenderer/ZtDevice.hpp"
#include "Zinet/VulkanRenderer/ZtPipelineLayout.hpp"
#include "Zinet/VulkanRenderer/ZtRenderPass.hpp"
#include "Zinet/VulkanRenderer/ZtDrawInfo.hpp"
#include "Zinet/VulkanRenderer/ZtShaderModule.hpp"
#include "Zinet/VulkanRenderer/ZtBuffer.hpp"

namespace zt::vulkan_renderer
{

	VkPipelineDynamicStateCreateInfo Pipeline::createVkPipelineDynamicStateCreateInfo(
		const std::vector<VkDynamicState>& dynamicStates) const noexcept
	{
		return VkPipelineDynamicStateCreateInfo
		{
			.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
			.dynamicStateCount = static_cast<std::uint32_t>(dynamicStates.size()),
			.pDynamicStates = dynamicStates.data()
		};
	}

	VkPipelineVertexInputStateCreateInfo Pipeline::createVkPipelineVertexInputStateCreateInfo(
		const InputBindingDescription* bindingDescription,
		const InputAttributesDescriptions* attributesDescriptions) const noexcept
	{
		VkPipelineVertexInputStateCreateInfo createInfo
		{
			.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
			.pNext = nullptr,
			.flags = {}
		};

		if (bindingDescription && attributesDescriptions)
		{
			createInfo.vertexBindingDescriptionCount = 1;
			createInfo.pVertexBindingDescriptions = bindingDescription;
			createInfo.vertexAttributeDescriptionCount = static_cast<std::uint32_t>(attributesDescriptions->size());
			createInfo.pVertexAttributeDescriptions = attributesDescriptions->data();
		}
		else
		{
			createInfo.vertexBindingDescriptionCount = 0;
			createInfo.pVertexBindingDescriptions = nullptr;
			createInfo.vertexAttributeDescriptionCount = 0;
			createInfo.pVertexAttributeDescriptions = nullptr;
		}

		return createInfo;
	}

	VkPipelineInputAssemblyStateCreateInfo Pipeline::createVkPipelineInputAssemblyStateCreateInfo() const noexcept
	{
		return VkPipelineInputAssemblyStateCreateInfo
		{
			.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
			.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
			.primitiveRestartEnable = VK_FALSE
		};
	}

	VkPipelineViewportStateCreateInfo Pipeline::createVkPipelineViewportStateCreateInfo(
		const VkViewport& viewport, const VkRect2D& scissor) const noexcept
	{
		return VkPipelineViewportStateCreateInfo
		{
			.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
			.pNext = nullptr,
			.flags = {},
			.viewportCount = 1,
			.pViewports = &viewport,
			.scissorCount = 1,
			.pScissors = &scissor
		};
	}

	VkPipelineRasterizationStateCreateInfo Pipeline::createVkPipelineRasterizationStateCreateInfo() const noexcept
	{
		return VkPipelineRasterizationStateCreateInfo
		{
			.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
			.pNext = nullptr,
			.flags = {},
			.depthClampEnable = VK_FALSE,
			.rasterizerDiscardEnable = VK_FALSE,
			.polygonMode = VK_POLYGON_MODE_FILL,
			.cullMode = VK_CULL_MODE_BACK_BIT,
			.frontFace = VK_FRONT_FACE_CLOCKWISE,
			.depthBiasEnable = VK_FALSE,
			.depthBiasConstantFactor = 0.0f,
			.depthBiasClamp = 0.0f,
			.depthBiasSlopeFactor = 0.0f,
			.lineWidth = 1.0f
		};
	}

	VkPipelineMultisampleStateCreateInfo Pipeline::createVkPipelineMultisampleStateCreateInfo() const noexcept
	{
		return VkPipelineMultisampleStateCreateInfo
		{
			.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
			.pNext = nullptr,
			.flags = {},
			.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
			.sampleShadingEnable = VK_FALSE,
			.minSampleShading = 1.0f,
			.pSampleMask = nullptr,
			.alphaToCoverageEnable = VK_FALSE,
			.alphaToOneEnable = VK_FALSE
		};
	}

	VkPipelineColorBlendAttachmentState Pipeline::createVkPipelineColorBlendAttachmentState() const noexcept
	{
		return VkPipelineColorBlendAttachmentState
		{
			.blendEnable = VK_TRUE,
			.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA,
			.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
			.colorBlendOp = VK_BLEND_OP_ADD,
			.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
			.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
			.alphaBlendOp = VK_BLEND_OP_ADD,
			.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT
		};
	}

	VkPipelineColorBlendStateCreateInfo Pipeline::createVkPipelineColorBlendStateCreateInfo(
		const VkPipelineColorBlendAttachmentState& colorBlendAttachmentState) const noexcept
	{
		return VkPipelineColorBlendStateCreateInfo
		{
			.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
			.pNext = nullptr,
			.flags = {},
			.logicOpEnable = VK_FALSE,
			.logicOp = VK_LOGIC_OP_COPY,
			.attachmentCount = 1,
			.pAttachments = &colorBlendAttachmentState,
			.blendConstants = { 0.0f, 0.0f, 0.0f, 0.0f }
		};
	}

	bool Pipeline::create(
		const Device& device,
		const PipelineLayout& pipelineLayout,
		const RenderPass& renderPass,
		const VkViewport& viewport,
		const VkRect2D& scissor,
		const DrawInfo& drawInfo)
	{
		if (isValid())
			return false;

		const std::vector<VkDynamicState> dynamicStates = {
			VK_DYNAMIC_STATE_VIEWPORT,
			VK_DYNAMIC_STATE_SCISSOR
		};

		const auto vertexInputBindingDescription = GetVertexInputBindingDescription();
		const auto vertexInputAttributesDescriptions = GetVertexInputAttributesDescriptions();

		VkPipelineVertexInputStateCreateInfo vertexInputStateCreateInfo;
		if (drawInfo.vertexBuffer && drawInfo.vertexBuffer->isValid())
		{
			vertexInputStateCreateInfo = createVkPipelineVertexInputStateCreateInfo(&vertexInputBindingDescription, &vertexInputAttributesDescriptions);
		}
		else
		{
			vertexInputStateCreateInfo = createVkPipelineVertexInputStateCreateInfo(nullptr, nullptr);
		}

		const auto dynamicStateCreateInfo = createVkPipelineDynamicStateCreateInfo(dynamicStates);
		const auto inputAssemblyStateCreateInfo = createVkPipelineInputAssemblyStateCreateInfo();
		const auto viewportStateCreateInfo = createVkPipelineViewportStateCreateInfo(viewport, scissor);
		const auto rasterizationStateCreateInfo = createVkPipelineRasterizationStateCreateInfo();
		const auto multisampleStateCreateInfo = createVkPipelineMultisampleStateCreateInfo();
		const auto colorBlendAttachmentState = createVkPipelineColorBlendAttachmentState();
		const auto colorBlendStateCreateInfo = createVkPipelineColorBlendStateCreateInfo(colorBlendAttachmentState);

		const auto shadersStages = createShadersStages(drawInfo);

		const VkGraphicsPipelineCreateInfo createInfo
		{
			.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
			.pNext = nullptr,
			.flags = {},
			.stageCount = static_cast<std::uint32_t>(shadersStages.size()),
			.pStages = shadersStages.data(),
			.pVertexInputState = &vertexInputStateCreateInfo,
			.pInputAssemblyState = &inputAssemblyStateCreateInfo,
			.pViewportState = &viewportStateCreateInfo,
			.pRasterizationState = &rasterizationStateCreateInfo,
			.pMultisampleState = &multisampleStateCreateInfo,
			.pDepthStencilState = nullptr,
			.pColorBlendState = &colorBlendStateCreateInfo,
			.pDynamicState = &dynamicStateCreateInfo,
			.layout = pipelineLayout.get(),
			.renderPass = renderPass.get(),
			.subpass = 0,
			.basePipelineHandle = VK_NULL_HANDLE,
			.basePipelineIndex = -1
		};

		const auto result = vkCreateGraphicsPipelines(device.get(), VK_NULL_HANDLE, 1, &createInfo, nullptr, &get());
		if (result == VK_SUCCESS)
		{
			return true;
		}
		else
		{
			Logger->error("Couldn't create Pipeline, result: {}", static_cast<std::int32_t>(result));
			return false;
		}
	}

	void Pipeline::destroy(const Device& device) noexcept
	{
		if (isValid())
		{
			vkDestroyPipeline(device.get(), get(), nullptr);
			invalidateInternal();
		}
	}

	Pipeline::ShadersStages Pipeline::createShadersStages(const DrawInfo& drawInfo) const
	{
		Pipeline::ShadersStages shadersStages;

		if (drawInfo.vertexShaderModule && drawInfo.vertexShaderModule->isValid())
		{
			shadersStages.push_back(drawInfo.vertexShaderModule->createPipelineShaderStageCreateInfo(ShaderType::Vertex));
		}
		else
		{
			Logger->error("Vertex shader module is invalid");
		}

		if (drawInfo.fragmentShaderModule && drawInfo.fragmentShaderModule->isValid())
		{
			shadersStages.push_back(drawInfo.fragmentShaderModule->createPipelineShaderStageCreateInfo(ShaderType::Fragment));
		}
		else
		{
			Logger->error("Fragment shader module is invalid");
		}

		return shadersStages;
	}

}