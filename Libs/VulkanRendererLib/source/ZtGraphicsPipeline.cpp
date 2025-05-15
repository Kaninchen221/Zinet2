#include "Zinet/VulkanRenderer/ZtGraphicsPipeline.hpp"
#include "Zinet/VulkanRenderer/ZtRendererContext.hpp"
#include "Zinet/VulkanRenderer/ZtDrawInfo.hpp"
#include "Zinet/VulkanRenderer/ZtShaderModule.hpp"
#include "Zinet/VulkanRenderer/ZtBuffer.hpp"

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

		// Descriptor Pool - One for entire program like command pool?
		const auto descriptorPoolSize = DescriptorPool::GetDefaultDescriptorPoolSize();
		std::vector<VkDescriptorPoolSize> descriptorPoolSizes{ descriptorPoolSize };
		const auto descriptorPoolCreateInfo = DescriptorPool::GetDefaultCreateInfo(descriptorPoolSizes);
		if (!descriptorPool.create(device, descriptorPoolCreateInfo))
			return false;

		// Uniforms section
		const auto layoutBinding = DescriptorSetLayout::GetDefaultLayoutBinding();
		const DescriptorSetLayout::Bindings bindings{ layoutBinding };

		const auto createInfo = DescriptorSetLayout::GetDefaultCreateInfo(bindings);
		if (!descriptorSetLayout.create(createInfo, device))
			return false;

		const std::vector descriptorSetLayouts{ descriptorSetLayout.get() };
		const auto allocateInfo = DescriptorSet::GetDefaultAllocateInfo(descriptorPool, descriptorSetLayouts);
		if (!descriptorSet.create(device, allocateInfo))
			return false;

		// Pipeline Layout must be created after uniforms
		auto pipelineLayoutCreateInfo = PipelineLayout::GetDefaultCreateInfo();
		pipelineLayoutCreateInfo.pSetLayouts = descriptorSetLayouts.data();
		pipelineLayoutCreateInfo.setLayoutCount = static_cast<std::uint32_t>(descriptorSetLayouts.size());
		if (!pipelineLayout.create(device, pipelineLayoutCreateInfo))
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
	}

	void GraphicsPipeline::draw(const RendererContext& rendererContext, const DrawInfo& drawInfo)
	{
		auto& swapChain = rendererContext.swapChain;
		auto& device = rendererContext.device;
		auto& queue = rendererContext.queue;

		// Pre Draw
		if (!pipeline.isValid())
		{
			const auto extent = swapChain.getExtent();
			VkViewport viewport{ 0, 0, static_cast<float>(extent.width), static_cast<float>(extent.height) };
			VkRect2D scissor{ { 0, 0 }, extent };
			if (!pipeline.create(device, pipelineLayout, renderPass, viewport, scissor, drawInfo))
				return;
		}

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

		// Uniform Buffers

		// Update descriptor set
		VkDescriptorBufferInfo descriptorBufferInfo = DescriptorSet::GetBufferInfo(drawInfo.uniformBuffer); // TODO: Support more than one uniform

		auto writeDescriptorSet = DescriptorSet::GetDefaultWriteDescriptorSet();
		writeDescriptorSet.pBufferInfo = &descriptorBufferInfo;
		writeDescriptorSet.dstSet = descriptorSet.get();
		descriptorSet.update(device, writeDescriptorSet);

		if (descriptorSet.isValid())
		{
			const auto vkDescriptorSet = descriptorSet.get();
			vkCmdBindDescriptorSets(
				commandBuffer.get(),
				VK_PIPELINE_BIND_POINT_GRAPHICS,
				pipelineLayout.get(),
				0,
				1,
				&vkDescriptorSet,
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

		// Post Draw
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