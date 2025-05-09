#include "Zinet/VulkanRenderer/ZtGraphicsPipeline.hpp"
#include "Zinet/VulkanRenderer/ZtRendererContext.hpp"
#include "Zinet/VulkanRenderer/ZtDrawInfo.hpp"
#include "Zinet/VulkanRenderer/ZtShaderModule.hpp"
#include "Zinet/VulkanRenderer/ZtBuffer.hpp"

namespace zt::vulkan_renderer
{

	bool GraphicsPipeline::create(RendererContext& rendererContext) noexcept
	{
		auto& device = rendererContext.device;
		auto& swapChain = rendererContext.swapChain;
		auto& commandPool = rendererContext.commandPool;

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

		if (!pipelineLayout.create(device))
			return false;

		return true;
	}

	void GraphicsPipeline::destroy(const RendererContext& rendererContext) noexcept
	{
		auto& device = rendererContext.device;

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

		///////
		const VkBuffer vertexBuffers[] = { drawInfo.vertexBuffer.get() };
		const VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer.get(), 0, 1, vertexBuffers, offsets);
		///////

		if (drawInfo.indexBuffer.isValid())
		{
			vkCmdBindIndexBuffer(commandBuffer.get(), drawInfo.indexBuffer.get(), 0, VK_INDEX_TYPE_UINT16);
			vkCmdDrawIndexed(commandBuffer.get(), drawInfo.indexCount, 1, 0, 0, 0);
		}
		else
		{
			commandBuffer.draw(static_cast<std::uint32_t>(drawInfo.vertexBuffer.getSize()), 1, 0, 0);
		}

		commandBuffer.endRenderPass();
		commandBuffer.end();

		//
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

		//

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