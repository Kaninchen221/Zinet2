#include "Zinet/VulkanRenderer/ZtGraphicsPipeline.hpp"
#include "Zinet/VulkanRenderer/ZtRendererContext.hpp"
#include "Zinet/VulkanRenderer/ZtDrawInfo.hpp"
#include "Zinet/VulkanRenderer/ZtShaderModule.hpp"
#include "Zinet/VulkanRenderer/ZtBuffer.hpp"
#include "Zinet/VulkanRenderer/ZtTexture.hpp"

namespace zt::vulkan_renderer
{
	bool GraphicsPipeline::create(const RendererContext& rendererContext, const DrawInfo& drawInfo) noexcept
	{
		auto& device = rendererContext.device;
		auto& swapChain = rendererContext.swapChain;
		auto& renderPass = rendererContext.renderPass;
		auto& commandPool = rendererContext.commandPool;

		auto& pipelineDescriptorInfo = drawInfo.pipelineDescriptorInfo;
		auto& objectDescriptorInfo = drawInfo.objectDescriptorInfo;

		if (!commandBuffer.create(device, commandPool))
			return false;

		DescriptorSetLayout::Bindings pipelineBindings;
		DescriptorSetLayout::Bindings objectBindings;

		DescriptorPoolSizes descriptorPoolSizes;

		createDescriptorData(pipelineBindings, descriptorPoolSizes, pipelineDescriptorInfo);

		createDescriptorData(objectBindings, descriptorPoolSizes, objectDescriptorInfo);

		if (!createDescriptorSetLayout(device, pipelineBindings))
		{
			Logger->error("Couldn't create pipeline descriptor set layout");
			return false;
		}

		if (!createDescriptorSetLayout(device, objectBindings))
		{
			Logger->error("Couldn't create object descriptor set layout");
			return false;
		}

		const auto descriptorPoolCreateInfo = DescriptorPool::GetDefaultCreateInfo(descriptorPoolSizes);
		if (!descriptorPool.create(device, descriptorPoolCreateInfo))
			return false;

		const auto vkDescriptorSetLayouts = VulkanObjectsToVkObjects<std::vector<VkDescriptorSetLayout>>(descriptorSetLayouts);

		const auto allocateInfo = DescriptorSets::GetDefaultAllocateInfo(descriptorPool, vkDescriptorSetLayouts);
		if (!descriptorSets.create(device, allocateInfo))
			return false;

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

		descriptorPool.destroy(device);
		destroyAll(descriptorSetLayouts, device);
		descriptorSets.invalidate();

		pipeline.destroy(device);
		pipelineLayout.destroy(device);

		commandBuffer.invalidate();
	}

	void GraphicsPipeline::draw(const RendererContext& rendererContext, const DrawInfo& drawInfo) noexcept
	{
		auto& swapChain = rendererContext.swapChain;
		auto& device = rendererContext.device;
		auto currentFramebufferIndex = rendererContext.currentFramebufferIndex;
		auto& currentFramebuffer = rendererContext.framebuffers[currentFramebufferIndex];
		auto& renderPass = rendererContext.renderPass;

		auto& pipelineDescriptorInfo = drawInfo.pipelineDescriptorInfo;

		commandBuffer.reset();

		commandBuffer.begin();
		const auto extent = swapChain.getExtent();
		commandBuffer.beginRenderPass(renderPass, currentFramebuffer, extent);

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

		// Update descriptor set
		std::vector<VkWriteDescriptorSet> writeDescriptorSets;
		std::vector<VkDescriptorBufferInfo> descriptorBuffersInfos;
		std::vector<VkDescriptorImageInfo> descriptorImagesInfos;
		if (pipelineDescriptorInfo.uniformBuffer->isValid())
		{
			auto& descriptorBufferInfo = descriptorBuffersInfos.emplace_back(DescriptorSets::GetBufferInfo(*pipelineDescriptorInfo.uniformBuffer));
			auto& writeDescriptorSet = writeDescriptorSets.emplace_back(DescriptorSets::GetDefaultWriteDescriptorSet());
			writeDescriptorSet.pBufferInfo = &descriptorBufferInfo;
			writeDescriptorSet.dstSet = descriptorSets.get();
		}

		if (!pipelineDescriptorInfo.texturesInfos.empty())
		{
			const auto textureInfo = pipelineDescriptorInfo.texturesInfos[0];
			auto& imageDescriptorInfo = descriptorImagesInfos.emplace_back(DescriptorSets::GetImageInfo(textureInfo.texture->getImageView(), *textureInfo.sampler));

			auto& writeDescriptorSet = writeDescriptorSets.emplace_back(DescriptorSets::GetDefaultWriteDescriptorSet());
			writeDescriptorSet.dstSet = descriptorSets.get();
			writeDescriptorSet.dstBinding = 1;
			writeDescriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			writeDescriptorSet.pImageInfo = &imageDescriptorInfo;
		}
		descriptorSets.update(device, writeDescriptorSets);

		if (descriptorSets.isValid())
		{
			const auto vkDescriptorSets = std::vector{ descriptorSets.get() };
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

		// Vertex Buffer
		const VkBuffer vertexBuffers[] = { drawInfo.vertexBuffer->get() };
		const VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer.get(), 0, 1, vertexBuffers, offsets);
		
		// Index Buffer
		if (drawInfo.indexBuffer->isValid())
		{
			vkCmdBindIndexBuffer(commandBuffer.get(), drawInfo.indexBuffer->get(), 0, VK_INDEX_TYPE_UINT16);
			vkCmdDrawIndexed(commandBuffer.get(), drawInfo.indexCount, 1, 0, 0, 0);
		}
		else
		{
			commandBuffer.draw(static_cast<std::uint32_t>(drawInfo.vertexBuffer->getSize()), 1, 0, 0);
		}

		commandBuffer.endRenderPass();
		commandBuffer.end();
	}

	bool GraphicsPipeline::submit(const RendererContext& rendererContext) noexcept
	{
		auto& queue = rendererContext.queue;
		auto& imageAvailableSemaphore = rendererContext.imageAvailableSemaphore;
		auto& renderFinishedSemaphore = rendererContext.renderFinishedSemaphore;
		auto& fence = rendererContext.fence;

		const VkSemaphore waitSemaphores[] = { imageAvailableSemaphore.get() };
		const VkPipelineStageFlags waitStages[] = {
			VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
			VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT
		};
		const auto vkCommandBuffer = commandBuffer.get();
		const VkSemaphore signalSemaphores[] = { renderFinishedSemaphore.get() };

		const VkSubmitInfo submitInfo
		{
			.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
			.waitSemaphoreCount = 1,
			.pWaitSemaphores = waitSemaphores,
			.pWaitDstStageMask = waitStages,
			.commandBufferCount = 1,
			.pCommandBuffers = &vkCommandBuffer,
			.signalSemaphoreCount = 1,
			.pSignalSemaphores = signalSemaphores
		};

		const auto result = vkQueueSubmit(queue.get(), 1, &submitInfo, fence.get());
			
		if (result != VK_SUCCESS)
		{
			Logger->error("vkQueueSubmit returned not VK_SUCCESS, error code: {}", static_cast<int>(result));
			return false;
		}

		return true;
	}

	bool GraphicsPipeline::isValid() const noexcept
	{
		return pipeline.isValid() && pipelineLayout.isValid() && descriptorSets.isValid();
	}

	void GraphicsPipeline::createDescriptorData(DescriptorSetLayout::Bindings& outBindings, DescriptorPoolSizes& outDescriptorPoolSizes, const DescriptorInfo& descriptorInfo) const noexcept
	{
		if (descriptorInfo.uniformBuffer && descriptorInfo.uniformBuffer->isValid())
		{
			auto uniformLayoutBinding = DescriptorSetLayout::GetDefaultUniformLayoutBinding();
			uniformLayoutBinding.binding = static_cast<uint32_t>(outBindings.size());
			outBindings.push_back(uniformLayoutBinding);

			auto descriptorPoolSize = DescriptorPool::GetDefaultDescriptorPoolSize();
			descriptorPoolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			outDescriptorPoolSizes.push_back(descriptorPoolSize);
		}

		// TODO: Handle more textures
		if (!descriptorInfo.texturesInfos.empty())
		{
			auto imageLayoutBinding = DescriptorSetLayout::GetDefaultImageLayoutBinding();
			imageLayoutBinding.binding = static_cast<uint32_t>(outBindings.size());
			outBindings.push_back(imageLayoutBinding);

			auto descriptorPoolSize = DescriptorPool::GetDefaultDescriptorPoolSize();
			descriptorPoolSize.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			outDescriptorPoolSizes.push_back(descriptorPoolSize);
		}
	}

	bool GraphicsPipeline::createDescriptorSetLayout(const Device& device, DescriptorSetLayout::Bindings& bindings) noexcept
	{
		if (bindings.empty())
			return true;

		const auto descriptorSetLayoutCreateInfo = DescriptorSetLayout::GetDefaultCreateInfo(bindings);
		auto& descriptorSetLayout = descriptorSetLayouts.emplace_back(nullptr);
		if (!descriptorSetLayout.create(descriptorSetLayoutCreateInfo, device))
			return false;

		return true;
	}

}