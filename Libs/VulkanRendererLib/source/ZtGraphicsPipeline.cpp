#include "Zinet/VulkanRenderer/ZtGraphicsPipeline.hpp"
#include "Zinet/VulkanRenderer/ZtRendererContext.hpp"
#include "Zinet/VulkanRenderer/ZtDrawInfo.hpp"
#include "Zinet/VulkanRenderer/ZtShaderModule.hpp"
#include "Zinet/VulkanRenderer/ZtBuffer.hpp"
#include "Zinet/VulkanRenderer/ZtTexture.hpp"

namespace zt::vulkan_renderer
{
	bool GraphicsPipeline::create(const RendererContext& rendererContext, DrawInfo& drawInfo) noexcept
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
		pipelineDescriptorSetLayout = createDescriptorSetLayout(device, pipelineBindings);

		createDescriptorData(objectBindings, descriptorPoolSizes, objectDescriptorInfo);
		objectDescriptorSetLayout = createDescriptorSetLayout(device, objectBindings);

		if (!descriptorPoolSizes.empty())
		{
			const auto descriptorPoolCreateInfo = DescriptorPool::GetDefaultCreateInfo(descriptorPoolSizes);
			if (!descriptorPool.create(device, descriptorPoolCreateInfo))
				return false;
		}

		std::vector<VkDescriptorSetLayout> vkDescriptorSetLayouts;
		pipelineDescriptorSet = createDescriptorSet(device, pipelineDescriptorSetLayout, vkDescriptorSetLayouts);
		objectDescriptorSet = createDescriptorSet(device, objectDescriptorSetLayout, vkDescriptorSetLayouts);

		UpdateDescriptorSet(device, drawInfo.pipelineDescriptorInfo, pipelineDescriptorSet);

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
		pipelineDescriptorSetLayout.destroy(device);
		pipelineDescriptorSet.invalidate();
		objectDescriptorSetLayout.destroy(device);
		objectDescriptorSet.invalidate();

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

		UpdateDescriptorSet(device, drawInfo.objectDescriptorInfo, objectDescriptorSet);

		if (objectDescriptorSet.isValid())
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
		// Ignore objects related to descriptors
		return 
			commandBuffer.isValid() &&
			pipeline.isValid() &&
			pipelineLayout.isValid();
	}

	void GraphicsPipeline::createDescriptorData(DescriptorSetLayout::Bindings& outBindings, DescriptorPoolSizes& outDescriptorPoolSizes, DescriptorInfo& descriptorInfo) const noexcept
	{
		auto& uniformBuffer = descriptorInfo.uniformBuffer;
		if (uniformBuffer && uniformBuffer->isValid())
		{
			auto layoutBinding = DescriptorSetLayout::GetDefaultUniformLayoutBinding();
			layoutBinding.binding = static_cast<uint32_t>(outBindings.size());
			descriptorInfo.uniformCachedBinding = layoutBinding.binding;
			outBindings.push_back(layoutBinding);

			auto descriptorPoolSize = DescriptorPool::GetDefaultDescriptorPoolSize();
			descriptorPoolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			outDescriptorPoolSizes.push_back(descriptorPoolSize);
		}

		for (auto& textureInfo : descriptorInfo.texturesInfos)
		{
			auto layoutBinding = DescriptorSetLayout::GetDefaultImageLayoutBinding();
			layoutBinding.binding = static_cast<uint32_t>(outBindings.size());
			textureInfo.cachedBinding = layoutBinding.binding;
			outBindings.push_back(layoutBinding);

			auto poolSize = DescriptorPool::GetDefaultDescriptorPoolSize();
			poolSize.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			outDescriptorPoolSizes.push_back(poolSize);
		}
	}

	DescriptorSetLayout GraphicsPipeline::createDescriptorSetLayout(const Device& device, DescriptorSetLayout::Bindings& bindings) noexcept
	{
		if (bindings.empty())
			return DescriptorSetLayout{ nullptr };

		const auto descriptorSetLayoutCreateInfo = DescriptorSetLayout::GetDefaultCreateInfo(bindings);
		DescriptorSetLayout descriptorSetLayout{ nullptr };
		descriptorSetLayout.create(descriptorSetLayoutCreateInfo, device);

		return std::move(descriptorSetLayout);
	}

	DescriptorSets GraphicsPipeline::createDescriptorSet(
		const Device& device,
		const DescriptorSetLayout& layout,
		std::vector<VkDescriptorSetLayout>& outLayouts) noexcept
	{
		DescriptorSets descriptorSet{ nullptr };

		if (layout.isValid())
		{
			const std::vector<VkDescriptorSetLayout> vkDescriptorSetLayouts{ layout.get() };
			outLayouts.push_back(layout.get());
			const auto allocateInfo = DescriptorSets::GetDefaultAllocateInfo(descriptorPool, vkDescriptorSetLayouts);
			if (descriptorSet.create(device, allocateInfo))
			{
				vkDescriptorSets.push_back(descriptorSet.get());
			}
		}

		return std::move(descriptorSet);
	}

	void GraphicsPipeline::UpdateDescriptorSet(const Device& device, const DescriptorInfo& descriptorInfo, const DescriptorSets& descriptorSet) noexcept
	{
		std::vector<VkWriteDescriptorSet> writeDescriptorSets;
		std::vector<VkDescriptorBufferInfo> descriptorBuffersInfos;
		std::vector<VkDescriptorImageInfo> descriptorImagesInfos;

		auto& uniformBuffer = descriptorInfo.uniformBuffer;
		if (uniformBuffer && uniformBuffer->isValid())
		{
			auto& descriptorBufferInfo = descriptorBuffersInfos.emplace_back(DescriptorSets::GetBufferInfo(*descriptorInfo.uniformBuffer));
			auto& writeDescriptorSet = writeDescriptorSets.emplace_back(DescriptorSets::GetDefaultWriteDescriptorSet());
			writeDescriptorSet.dstSet = descriptorSet.get();
			writeDescriptorSet.dstBinding = descriptorInfo.uniformCachedBinding;
			writeDescriptorSet.pBufferInfo = &descriptorBufferInfo;
		}

		for (auto& textureInfo : descriptorInfo.texturesInfos)
		{
			auto& imageDescriptorInfo = descriptorImagesInfos.emplace_back(DescriptorSets::GetImageInfo(textureInfo.texture->getImageView(), *textureInfo.sampler));

			auto& writeDescriptorSet = writeDescriptorSets.emplace_back(DescriptorSets::GetDefaultWriteDescriptorSet());
			writeDescriptorSet.dstSet = descriptorSet.get();
			writeDescriptorSet.dstBinding = textureInfo.cachedBinding;
			writeDescriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			writeDescriptorSet.pImageInfo = &imageDescriptorInfo;
		}

		if (!writeDescriptorSets.empty())
			descriptorSet.update(device, writeDescriptorSets);
	}

}