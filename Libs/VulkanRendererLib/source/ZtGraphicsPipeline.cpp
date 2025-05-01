#include "Zinet/VulkanRenderer/ZtGraphicsPipeline.hpp"
#include "Zinet/VulkanRenderer/ZtRendererContext.hpp"
#include "Zinet/VulkanRenderer/ZtDrawInfo.hpp"
#include "Zinet/VulkanRenderer/ZtShaderModule.hpp"

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
			Pipeline::ShadersStages shadersStages;
			shadersStages.push_back(drawInfo.vertexShaderModule.createPipelineShaderStageCreateInfo(ShaderType::Vertex));
			shadersStages.push_back(drawInfo.fragmentShaderModule.createPipelineShaderStageCreateInfo(ShaderType::Fragment));

			const auto extent = swapChain.getExtent();
			VkViewport viewport{ 0, 0, static_cast<float>(extent.width), static_cast<float>(extent.height) };
			VkRect2D scissor{ { 0, 0 }, extent };
			if (!pipeline.create(device, pipelineLayout, renderPass, viewport, scissor, shadersStages))
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

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(swapChain.getExtent().width);
		viewport.height = static_cast<float>(swapChain.getExtent().height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		commandBuffer.setViewport(viewport);

		VkRect2D scissor{};
		scissor.offset = { 0, 0 };
		scissor.extent = swapChain.getExtent();
		commandBuffer.setScissor(scissor);

		commandBuffer.draw(3, 1, 0, 0);

		commandBuffer.endRenderPass();
		commandBuffer.end();

		//

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		VkSemaphore waitSemaphores[] = { imageAvailableSemaphore.get() };
		VkPipelineStageFlags waitStages[] = {
			VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
			VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT
		};
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;

		submitInfo.commandBufferCount = 1;
		auto vkCommandBuffer = commandBuffer.get();
		submitInfo.pCommandBuffers = &vkCommandBuffer;

		VkSemaphore signalSemaphores[] = { renderFinishedSemaphore.get() };
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;

		vkQueueSubmit(queue.get(), 1, &submitInfo, fence.get());

		//

		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;

		VkSwapchainKHR swapChains[] = { swapChain.get() };
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;
		presentInfo.pImageIndices = &imageIndex;
		presentInfo.pResults = nullptr; // Optional

		vkQueuePresentKHR(queue.get(), &presentInfo);

	}

}