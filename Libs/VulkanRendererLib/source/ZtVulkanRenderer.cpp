#include "Zinet/VulkanRenderer/ZtVulkanRenderer.hpp"
#include "Zinet/VulkanRenderer/ZtShaderModule.hpp"

#include "Zinet/Window/ZtWindow.hpp"

namespace zt::vulkan_renderer
{

	bool VulkanRenderer::init(wd::Window& window)
	{
		window.setWindowResizedCallback(this, WindowResizedCallback);

		if (!rendererContext.create(window))
			return false;

		return true;
	}

	void VulkanRenderer::deinit()
	{
		rendererContext.device.waitIdle();

		graphicsPipeline.destroy(rendererContext);
		rendererContext.destroy();
	}

	bool VulkanRenderer::nextFrame()
	{
		auto& swapChain = rendererContext.swapChain;
		auto& device = rendererContext.device;
		auto& currentFramebufferIndex = rendererContext.currentFramebufferIndex;

		currentFramebufferIndex = swapChain.acquireNextImage(device, rendererContext.imageAvailableSemaphore);
		if (currentFramebufferIndex == SwapChain::InvalidIndex)
			return false;

		auto& fence = rendererContext.getCurrentDisplayImage().fence;

		if (!fence.wait(device))
			return false;

		if (!fence.reset(device))
			return false;

		return true;
	}

	bool VulkanRenderer::createPipeline(DrawInfo& drawInfo)
	{
		if (graphicsPipeline.isValid())
			return true;

		return graphicsPipeline.create(rendererContext, drawInfo);
	}

	void VulkanRenderer::draw(const DrawInfo& drawInfo)
	{
		graphicsPipeline.draw(rendererContext, drawInfo);
	}

	bool VulkanRenderer::submitDrawInfo()
	{
		auto& queue = rendererContext.queue;
		auto& fence = rendererContext.getCurrentDisplayImage().fence;
		auto& commandBuffer = rendererContext.getCurrentDisplayImage().commandBuffer;

		std::vector<VkSemaphore> waitSemaphores = { rendererContext.imageAvailableSemaphore.get() };
		
		std::vector<VkPipelineStageFlags> waitStages = {
			VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
		};

		// Sanity check
#		if ZINET_DEBUG
		if (waitSemaphores.size() != waitStages.size())
		{
			Logger->error("waitSemaphores.size() != waitStages.size()");
			return false;
		}
#		endif // ZINET_DEBUG

		std::vector<VkCommandBuffer> vkCommandBuffers;
		vkCommandBuffers.push_back(commandBuffer.get());

		std::vector<VkSemaphore> signalSemaphores;
		signalSemaphores.push_back(rendererContext.renderFinishedSemaphore.get());

		const VkSubmitInfo submitInfo
		{
			.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
			.waitSemaphoreCount = static_cast<uint32_t>(waitSemaphores.size()),
			.pWaitSemaphores = waitSemaphores.data(),
			.pWaitDstStageMask = waitStages.data(),
			.commandBufferCount = static_cast<uint32_t>(vkCommandBuffers.size()),
			.pCommandBuffers = vkCommandBuffers.data(),
			.signalSemaphoreCount = static_cast<uint32_t>(signalSemaphores.size()),
			.pSignalSemaphores = signalSemaphores.data()
		};

		// TODO: Fix the problem occuring on the laptop about semaphore
		const auto result = vkQueueSubmit(queue.get(), 1, &submitInfo, fence.get());

		if (result != VK_SUCCESS)
		{
			Logger->error("vkQueueSubmit returned not VK_SUCCESS, error code: {}", static_cast<int>(result));
			return false;
		}

		return true;
	}

	bool VulkanRenderer::displayCurrentFrame()
	{
		const auto& swapChain = rendererContext.swapChain;
		const auto& queue = rendererContext.queue;
		auto currentFramebufferIndex = rendererContext.currentFramebufferIndex;

		std::vector<VkSemaphore> waitSemaphores = { rendererContext.renderFinishedSemaphore.get() };
		std::vector<VkSwapchainKHR> swapChains = { swapChain.get() };
		const VkPresentInfoKHR presentInfo
		{
			.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
			.pNext = nullptr,
			.waitSemaphoreCount = static_cast<uint32_t>(waitSemaphores.size()),
			.pWaitSemaphores = waitSemaphores.data(),
			.swapchainCount = static_cast<uint32_t>(swapChains.size()),
			.pSwapchains = swapChains.data(),
			.pImageIndices = &currentFramebufferIndex,
			.pResults = nullptr
		};

		const auto result = vkQueuePresentKHR(queue.get(), &presentInfo);

		if (result != VK_SUCCESS)
		{
			Logger->error("vkQueuePresentKHR returned not VK_SUCCESS, error code: {}", static_cast<int>(result));
			return false;
		}

		return true;
	}

	void VulkanRenderer::WindowResizedCallback(void* userPointer, const Vector2i& size)
	{
		Logger->info("Window resized");

		VulkanRenderer& vulkanRenderer = *(VulkanRenderer*)userPointer;
		auto& rendererContext = vulkanRenderer.getRendererContext();

		auto& device = rendererContext.device;
		auto& graphicsPipeline = vulkanRenderer.getGraphicsPipeline();
		auto& fence = rendererContext.getCurrentDisplayImage().fence;

		device.waitIdle();
		fence.wait(device);

		rendererContext.windowResized(size);

		graphicsPipeline.destroy(rendererContext);
	}

}