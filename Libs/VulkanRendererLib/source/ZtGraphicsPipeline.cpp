#include "Zinet/VulkanRenderer/ZtGraphicsPipeline.hpp"
#include "Zinet/VulkanRenderer/ZtRendererContext.hpp"
#include "Zinet/VulkanRenderer/ZtDrawInfo.hpp"
#include "Zinet/VulkanRenderer/ZtShaderModule.hpp"
#include "Zinet/VulkanRenderer/ZtBuffer.hpp"
#include "Zinet/VulkanRenderer/ZtTexture.hpp"

namespace zt::vulkan_renderer
{
	bool GraphicsPipeline::create(const RendererContext& rendererContext, DrawInfo& drawInfo)
	{
		if (isValid())
			return false;

		auto& device = rendererContext.device;
		auto& swapChain = rendererContext.swapChain;
		auto& renderPass = rendererContext.renderPass;

		//auto& pipelineDescriptorInfo = drawInfo.pipelineDescriptorInfo;
		//auto& drawCallDescriptorInfo = drawInfo.drawCallDescriptorInfo;

		auto& currentDisplayImage = rendererContext.getCurrentDisplayImage();
		auto& pipelineDescriptorSetLayout = currentDisplayImage.pipelineDescriptorSetLayout;
		//auto& pipelineDescriptorSet = currentDisplayImage.pipelineDescriptorSet;
		auto& objectDescriptorSetLayout = currentDisplayImage.objectDescriptorSetLayout;
		//auto& objectDescriptorSet = currentDisplayImage.objectDescriptorSet;

		const std::vector<DescriptorSetLayout::HandleType>& vkDescriptorSetLayouts = 
		{
			pipelineDescriptorSetLayout.get(),
			objectDescriptorSetLayout.get()
		};

		auto pipelineLayoutCreateInfo = PipelineLayout::GetDefaultCreateInfo(vkDescriptorSetLayouts);
		if (!pipelineLayout.create(device, pipelineLayoutCreateInfo))
			return false;

		const auto extent = swapChain.getExtent();
		const VkViewport viewport{ 0, 0, static_cast<float>(extent.width), static_cast<float>(extent.height) };
		const VkRect2D scissor{ { 0, 0 }, extent };
		if (!pipeline.create(device, pipelineLayout, renderPass, viewport, scissor, drawInfo))
			return false;

		return true;
	}

	void GraphicsPipeline::destroy(const RendererContext& rendererContext) noexcept
	{
		const auto& device = rendererContext.device;

		pipeline.destroy(device);
		pipelineLayout.destroy(device);
	}

	void GraphicsPipeline::draw(RendererContext& rendererContext, const DrawInfo& drawInfo)
	{
		auto& swapChain = rendererContext.swapChain;
		auto& device = rendererContext.device;
		auto& displayImage = rendererContext.getCurrentDisplayImage();
		auto& renderPass = rendererContext.renderPass;
		auto& commandBuffer = displayImage.commandBuffer;
		auto& vkDescriptorSets = displayImage.vkDescriptorSets;
		auto& objectDescriptorSet = displayImage.objectDescriptorSet;
		auto& pipelineDescriptorSet = displayImage.pipelineDescriptorSet;

		// Begin draw start
		commandBuffer.reset();

		commandBuffer.begin();
		const auto extent = swapChain.getExtent();
		commandBuffer.beginRenderPass(renderPass, displayImage.framebuffer, extent);

		commandBuffer.bindPipeline(pipeline);

		// Viewport settings
		const VkViewport viewport
		{
			.x = 0.0f,
			.y = 0.0f,
			.width = static_cast<float>(swapChain.getExtent().width),
			.height = static_cast<float>(swapChain.getExtent().height),
			.minDepth = 0.0f,
			.maxDepth = 1.0f
		};
		commandBuffer.setViewport(viewport);

		const VkRect2D scissor
		{
			.offset = { 0, 0 },
			.extent = swapChain.getExtent()
		};
		commandBuffer.setScissor(scissor);

		if (displayImage.objectDescriptorSet.isValid())
		{
			vkCmdBindDescriptorSets(
				commandBuffer.get(),
				VK_PIPELINE_BIND_POINT_GRAPHICS,
				pipelineLayout.get(),
				0,
				static_cast<uint32_t>(vkDescriptorSets.size()),
				vkDescriptorSets.data(),
				0,
				nullptr);
		}
		// Begin draw end

		if (drawInfo.updatePipelineDescriptorInfoPerDrawCall)
			DisplayImage::UpdateDescriptorSet(device, drawInfo.pipelineDescriptorInfo, pipelineDescriptorSet);

		DisplayImage::UpdateDescriptorSet(device, drawInfo.drawCallDescriptorInfo, objectDescriptorSet);

		// Vertex Buffer
		const VkBuffer vertexBuffers[] = { drawInfo.vertexBuffer->get() };
		const VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer.get(), 0, 1, vertexBuffers, offsets);
		
		// Index Buffer
		if (drawInfo.indexBuffer->isValid())
		{
			vkCmdBindIndexBuffer(commandBuffer.get(), drawInfo.indexBuffer->get(), 0, VK_INDEX_TYPE_UINT16);
			vkCmdDrawIndexed(commandBuffer.get(), drawInfo.indexCount, drawInfo.instances, 0, 0, 0);
		}
		else
		{
			commandBuffer.draw(static_cast<uint32_t>(drawInfo.vertexBuffer->getSize()), drawInfo.instances, 0, 0);
		}

		for (const auto& additionalCommand : drawInfo.additionalCommands)
		{
			if (additionalCommand)
				additionalCommand.invoke(commandBuffer);
		}

		commandBuffer.endRenderPass();
		commandBuffer.end();
	}

	bool GraphicsPipeline::isValid() const noexcept
	{
		// Ignore objects related to descriptors
		return
			pipeline.isValid() &&
			pipelineLayout.isValid();
	}
}