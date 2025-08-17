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
		auto& commandPool = rendererContext.commandPool;

		auto& pipelineDescriptorInfo = drawInfo.pipelineDescriptorInfo;
		auto& drawCallDescriptorInfo = drawInfo.drawCallDescriptorInfo;

		if (!commandBuffer.create(device, commandPool))
			return false;

		DescriptorSetLayout::Bindings pipelineBindings;
		DescriptorSetLayout::Bindings objectBindings;

		DescriptorPoolSizes descriptorPoolSizes;

		createDescriptorData(pipelineBindings, descriptorPoolSizes, pipelineDescriptorInfo);
		pipelineDescriptorSetLayout = createDescriptorSetLayout(device, pipelineBindings);

		createDescriptorData(objectBindings, descriptorPoolSizes, drawCallDescriptorInfo);
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

		vkDescriptorSets.clear();

		pipeline.destroy(device);
		pipelineLayout.destroy(device);

		commandBuffer.invalidate();
	}

	void GraphicsPipeline::draw(const RendererContext& rendererContext, const DrawInfo& drawInfo)
	{
		auto& swapChain = rendererContext.swapChain;
		auto& device = rendererContext.device;
		auto currentFramebufferIndex = rendererContext.currentFramebufferIndex;
		auto& currentFramebuffer = rendererContext.displayImages.framebuffers[currentFramebufferIndex];
		auto& renderPass = rendererContext.renderPass;

		// Begin draw start
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
		// Begin draw end

		UpdateDescriptorSet(device, drawInfo.drawCallDescriptorInfo, objectDescriptorSet);

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
			std::invoke(additionalCommand, commandBuffer);
		}

		commandBuffer.endRenderPass();
		commandBuffer.end();
	}

	bool GraphicsPipeline::submit(const RendererContext& rendererContext) 
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

	void GraphicsPipeline::createDescriptorData(
		DescriptorSetLayout::Bindings& outBindings, 
		DescriptorPoolSizes& outDescriptorPoolSizes, 
		DescriptorInfo& descriptorInfo) const
	{
		auto& uniformBuffer = descriptorInfo.uniformBuffers;
		if (!uniformBuffer.empty())
		{
			descriptorInfo.cachedUniformBuffersBinding = static_cast<uint32_t>(outBindings.size());
			uint32_t descriptorsCount = static_cast<uint32_t>(descriptorInfo.uniformBuffers.size());

			auto layoutBinding = DescriptorSetLayout::GetDefaultUniformLayoutBinding();
			layoutBinding.binding = descriptorInfo.cachedUniformBuffersBinding;
			layoutBinding.descriptorCount = descriptorsCount;
			outBindings.push_back(layoutBinding);

			auto poolSize = DescriptorPool::GetDefaultDescriptorPoolSize();
			poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			poolSize.descriptorCount = descriptorsCount;
			outDescriptorPoolSizes.push_back(poolSize);
		}

		if (!descriptorInfo.texturesInfos.empty())
		{
			descriptorInfo.cachedTexturesBinding = static_cast<uint32_t>(outBindings.size());
			uint32_t descriptorsCount = static_cast<uint32_t>(descriptorInfo.texturesInfos.size());

			auto layoutBinding = DescriptorSetLayout::GetDefaultImageLayoutBinding();
			layoutBinding.binding = descriptorInfo.cachedTexturesBinding;
			layoutBinding.descriptorCount = descriptorsCount;
			outBindings.push_back(layoutBinding);

			auto poolSize = DescriptorPool::GetDefaultDescriptorPoolSize();
			poolSize.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			poolSize.descriptorCount = descriptorsCount;
			outDescriptorPoolSizes.push_back(poolSize);
		}
	}

	DescriptorSetLayout GraphicsPipeline::createDescriptorSetLayout(
		const Device& device, DescriptorSetLayout::Bindings& bindings)
	{
		if (bindings.empty())
			return DescriptorSetLayout{ nullptr };

		const auto descriptorSetLayoutCreateInfo = DescriptorSetLayout::GetDefaultCreateInfo(bindings);
		DescriptorSetLayout descriptorSetLayout{ nullptr };
		descriptorSetLayout.create(descriptorSetLayoutCreateInfo, device);

		return descriptorSetLayout;
	}

	DescriptorSets GraphicsPipeline::createDescriptorSet(
		const Device& device,
		const DescriptorSetLayout& layout,
		std::vector<VkDescriptorSetLayout>& outLayouts)
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

		return descriptorSet;
	}

	void GraphicsPipeline::UpdateDescriptorSet(
		const Device& device, const DescriptorInfo& descriptorInfo, const DescriptorSets& descriptorSet)
	{
		std::vector<VkWriteDescriptorSet> writeDescriptorSets;
		std::vector<VkDescriptorBufferInfo> descriptorBuffersInfos;
		std::vector<VkDescriptorImageInfo> descriptorImagesInfos;

		for (const auto& uniformBufferInfo : descriptorInfo.uniformBuffers)
		{
			auto& uniformBuffer = uniformBufferInfo.uniformBuffer;
			if (!uniformBuffer || !uniformBuffer->isValid())
				continue;

			auto& descriptorBufferInfo = descriptorBuffersInfos.emplace_back(DescriptorSets::GetBufferInfo(*uniformBuffer));
			descriptorBufferInfo.offset = 0;
		}

		/// Write Descriptor for uniform buffers
		if (!descriptorInfo.uniformBuffers.empty()) 
		{
			auto& writeDescriptorSet = writeDescriptorSets.emplace_back(DescriptorSets::GetDefaultWriteDescriptorSet());
			writeDescriptorSet.dstSet = descriptorSet.get();
			writeDescriptorSet.dstBinding = descriptorInfo.cachedUniformBuffersBinding;
			writeDescriptorSet.dstArrayElement = 0;
			writeDescriptorSet.descriptorCount = static_cast<uint32_t>(descriptorBuffersInfos.size());
			writeDescriptorSet.pBufferInfo = descriptorBuffersInfos.data();
			writeDescriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		}

		for (auto& textureInfo : descriptorInfo.texturesInfos)
		{
			if (!textureInfo.texture || !textureInfo.texture->isValid() || !textureInfo.sampler || !textureInfo.sampler->isValid())
				continue;

			[[maybe_unused]] 
			auto& imageDescriptorInfo = descriptorImagesInfos.emplace_back(DescriptorSets::GetImageInfo(textureInfo.texture->getImageView(), *textureInfo.sampler));
		}

		/// Write Descriptor for textures
		if (!descriptorInfo.texturesInfos.empty())
		{
			auto& writeDescriptorSet = writeDescriptorSets.emplace_back(DescriptorSets::GetDefaultWriteDescriptorSet());
			writeDescriptorSet.dstSet = descriptorSet.get();
			writeDescriptorSet.dstBinding = descriptorInfo.cachedTexturesBinding;
			writeDescriptorSet.dstArrayElement = 0;
			writeDescriptorSet.descriptorCount = static_cast<uint32_t>(descriptorImagesInfos.size());
			writeDescriptorSet.pImageInfo = descriptorImagesInfos.data();
			writeDescriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		}

		if (!writeDescriptorSets.empty())
			descriptorSet.update(device, writeDescriptorSets);
	}
}