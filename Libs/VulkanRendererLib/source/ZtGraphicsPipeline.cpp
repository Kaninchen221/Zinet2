#include "Zinet/VulkanRenderer/ZtGraphicsPipeline.hpp"
#include "Zinet/VulkanRenderer/ZtRendererContext.hpp"
#include "Zinet/VulkanRenderer/ZtDrawInfo.hpp"
#include "Zinet/VulkanRenderer/ZtShaderModule.hpp"
#include "Zinet/VulkanRenderer/ZtBuffer.hpp"
#include "Zinet/VulkanRenderer/ZtTexture.hpp"

namespace zt::vulkan_renderer
{

	bool GraphicsPipeline::create(RendererContext& rendererContext) noexcept
	{
		const auto& device = rendererContext.device;
		const auto& swapChain = rendererContext.swapChain;
		const auto& commandPool = rendererContext.commandPool;

		const auto renderPassCreateInfo = RenderPass::GetPresentCreateInfo(swapChain.getFormat());
		if (!renderPass.create(device, renderPassCreateInfo))
			return false;

		images = swapChain.getImages(device);
		if (images.empty())
		{
			Logger->error("SwapChain returned empty swapChainImages");
			return false;
		}

		for (auto image : images)
		{
			auto& imageView = imageViews.emplace_back(nullptr);
			const auto imageViewCreateInfo = ImageView::GetDefaultCreateInfo(image, swapChain.getFormat());
			if (!imageView.create(device, imageViewCreateInfo))
			{
				Logger->error("Couldn't create image view from one of the swap chain images");
				return false;
			}
		}

		const auto swapChainSize = swapChain.getExtent();
		const Vector2ui framebufferSize{ swapChainSize.width, swapChainSize.height };
		for (auto& imageView : imageViews)
		{
			auto& framebuffer = framebuffers.emplace_back(nullptr);
			if (!framebuffer.create(device, renderPass, imageView, framebufferSize))
			{
				Logger->error("Couldn't create framebuffer from one of the swap chain images");
				return false;
			}
		}

		if (!commandBuffer.create(device, commandPool))
			return false;

		if (!imageAvailableSemaphore.create(device))
			return false;

		if (!renderFinishedSemaphore.create(device))
			return false;

		if (!fence.create(device, true))
			return false;

		return true;
	}

	void GraphicsPipeline::destroy(const RendererContext& rendererContext) noexcept
	{
		const auto& device = rendererContext.device;

		// Uniforms
		descriptorPool.destroy(device);
		descriptorSetLayout.destroy(device);
		descriptorSet.invalidate();

		// Core
		for (auto& framebuffer : framebuffers)
			framebuffer.destroy(device);
		framebuffers.clear();

		for (auto& imageView : imageViews)
			imageView.destroy(device);
		imageViews.clear();

		renderPass.destroy(device);

		pipeline.destroy(device);
		pipelineLayout.destroy(device);

		imageAvailableSemaphore.destroy(device);

		renderFinishedSemaphore.destroy(device);

		fence.destroy(device);

		commandBuffer.invalidate();
	}

	bool GraphicsPipeline::preDraw(const RendererContext& rendererContext, const DrawInfo& drawInfo) noexcept
	{
		const auto& device = rendererContext.device;
		const auto& swapChain = rendererContext.swapChain;

		DescriptorSetLayout::Bindings bindings;
		std::vector<VkDescriptorPoolSize> descriptorPoolSizes;

		if (drawInfo.uniformBuffer.isValid())
		{
			auto uniformLayoutBinding = DescriptorSetLayout::GetDefaultUniformLayoutBinding();
			uniformLayoutBinding.binding = static_cast<uint32_t>(bindings.size());
			bindings.push_back(uniformLayoutBinding);

			auto descriptorPoolSize = DescriptorPool::GetDefaultDescriptorPoolSize();
			descriptorPoolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			descriptorPoolSizes.push_back(descriptorPoolSize);
		}

		if (!drawInfo.textureInfos.empty())
		{
			auto imageLayoutBinding = DescriptorSetLayout::GetDefaultImageLayoutBinding();
			imageLayoutBinding.binding = static_cast<uint32_t>(bindings.size());
			bindings.push_back(imageLayoutBinding);

			auto descriptorPoolSize = DescriptorPool::GetDefaultDescriptorPoolSize();
			descriptorPoolSize.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			descriptorPoolSizes.push_back(descriptorPoolSize);
		}

		const auto descriptorSetCreateInfo = DescriptorSetLayout::GetDefaultCreateInfo(bindings);
		if (!descriptorSetLayout.create(descriptorSetCreateInfo, device))
			return false;

		const auto descriptorPoolCreateInfo = DescriptorPool::GetDefaultCreateInfo(descriptorPoolSizes);
		if (!descriptorPool.create(device, descriptorPoolCreateInfo))
			return false;

		const std::vector descriptorSetLayouts{ descriptorSetLayout.get() };
		const auto allocateInfo = DescriptorSet::GetDefaultAllocateInfo(descriptorPool, descriptorSetLayouts);
		if (!descriptorSet.create(device, allocateInfo))
			return false;

		auto pipelineLayoutCreateInfo = PipelineLayout::GetDefaultCreateInfo(descriptorSetLayouts);
		if (!pipelineLayout.create(device, pipelineLayoutCreateInfo))
			return false;

		const auto extent = swapChain.getExtent();
		const VkViewport viewport{ 0, 0, static_cast<float>(extent.width), static_cast<float>(extent.height) };
		const VkRect2D scissor{ { 0, 0 }, extent };
		if (!pipeline.create(device, pipelineLayout, renderPass, viewport, scissor, drawInfo))
			return false;

		return true;
	}

	void GraphicsPipeline::draw(const RendererContext& rendererContext, const DrawInfo& drawInfo) noexcept
	{
		auto& swapChain = rendererContext.swapChain;
		auto& device = rendererContext.device;
		auto& queue = rendererContext.queue;

		fence.wait(device);
		fence.reset(device);

		const std::uint32_t imageIndex = swapChain.acquireNextImage(device, imageAvailableSemaphore);

		commandBuffer.reset();

		commandBuffer.begin();
		const auto extent = swapChain.getExtent();
		commandBuffer.beginRenderPass(renderPass, framebuffers[imageIndex], extent);

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
		if (drawInfo.uniformBuffer.isValid()) 
		{
			auto& descriptorBufferInfo = descriptorBuffersInfos.emplace_back(DescriptorSet::GetBufferInfo(drawInfo.uniformBuffer));
			auto& writeDescriptorSet = writeDescriptorSets.emplace_back(DescriptorSet::GetDefaultWriteDescriptorSet());
			writeDescriptorSet.pBufferInfo = &descriptorBufferInfo;
			writeDescriptorSet.dstSet = descriptorSet.get();
		}

		if (!drawInfo.textureInfos.empty())
		{
			const auto textureInfo = drawInfo.textureInfos[0];
			auto& imageDescriptorInfo = descriptorImagesInfos.emplace_back(DescriptorSet::GetImageInfo(textureInfo.texture->getImageView(), *textureInfo.sampler));

			auto& writeDescriptorSet = writeDescriptorSets.emplace_back(DescriptorSet::GetDefaultWriteDescriptorSet());
			writeDescriptorSet.dstSet = descriptorSet.get();
			writeDescriptorSet.dstBinding = 1;
			writeDescriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			writeDescriptorSet.pImageInfo = &imageDescriptorInfo;
		}
		descriptorSet.update(device, writeDescriptorSets);

		if (descriptorSet.isValid())
		{
			const auto vkDescriptorSets = std::vector{ descriptorSet.get() };
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
		const VkBuffer vertexBuffers[] = { drawInfo.vertexBuffer.get() };
		const VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer.get(), 0, 1, vertexBuffers, offsets);
		
		// Index Buffer
		if (drawInfo.indexBuffer.isValid())
		{
			vkCmdBindIndexBuffer(commandBuffer.get(), drawInfo.indexBuffer.get(), 0, VK_INDEX_TYPE_UINT16);
			vkCmdDrawIndexed(commandBuffer.get(), drawInfo.indexCount, 1, 0, 0, 0);
		}
		else
		{
			// Actual draw command
			commandBuffer.draw(static_cast<std::uint32_t>(drawInfo.vertexBuffer.getSize()), 1, 0, 0);
		}

		commandBuffer.endRenderPass();
		commandBuffer.end();

		// TODO: Post Draw?
		// Submit
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
		vkQueueSubmit(queue.get(), 1, &submitInfo, fence.get());

		// Present
		const VkSwapchainKHR swapChains[] = { swapChain.get() };
		const VkPresentInfoKHR presentInfo
		{
			.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
			.pNext = nullptr,
			.waitSemaphoreCount = 1,
			.pWaitSemaphores = signalSemaphores,
			.swapchainCount = 1,
			.pSwapchains = swapChains,
			.pImageIndices = &imageIndex,
			.pResults = nullptr
		};
		vkQueuePresentKHR(queue.get(), &presentInfo);

	}

}