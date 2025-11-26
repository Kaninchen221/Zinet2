#include "Zinet/VulkanRenderer/ZtVulkanRenderer.hpp"
#include "Zinet/VulkanRenderer/ZtShaderModule.hpp"

#include "Zinet/Window/ZtWindow.hpp"

namespace zt::vulkan_renderer
{

	bool VulkanRenderer::init(wd::Window& window)
	{
		wd::Window::SetWindowResizedCallback(this, WindowResizedCallback);

		if (!rendererContext.create(window))
			return false;

		initialized = true;
		return true;
	}

	void VulkanRenderer::deinit()
	{
		if (isInitialized())
		{
			rendererContext.device.waitIdle();

			rendererContext.destroy();
			initialized = false;
		}
	}

	bool VulkanRenderer::nextImage()
	{
		auto& swapChain = rendererContext.swapChain;
		auto& device = rendererContext.device;
		auto& currentDisplayImageIndex = rendererContext.currentDisplayImageIndex;
		auto& imageAvailableSemaphore = rendererContext.getNextDisplayImage().imageAvailableSemaphore;

		auto& fence = rendererContext.getNextDisplayImage().fence;

		if (!fence.wait(device))
			return false;

		if (!fence.reset(device))
			return false;

		currentDisplayImageIndex = swapChain.acquireNextImage(device, imageAvailableSemaphore);
		if (currentDisplayImageIndex == SwapChain::InvalidIndex)
			return false;

		auto& nextDisplayImageIndex = rendererContext.nextDisplayImageIndex;
		nextDisplayImageIndex = currentDisplayImageIndex + 1;
		if (nextDisplayImageIndex >= rendererContext.framesInFlight)
			nextDisplayImageIndex = 0;

		Logger->debug("Current display image index: {}", currentDisplayImageIndex);
		Logger->debug("Next display image index: {}", nextDisplayImageIndex);

		return true;
	}

	void VulkanRenderer::startRecordingDrawCommands()
	{
		auto& swapChain = rendererContext.swapChain;
		auto& displayImage = rendererContext.getCurrentDisplayImage();
		auto& commandBuffer = displayImage.commandBuffer;

		commandBuffer.reset();

		commandBuffer.begin();

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
	}

	void VulkanRenderer::beginRenderPass(RenderPass& renderPass)
	{
		auto& swapChain = rendererContext.swapChain;
		auto& displayImage = rendererContext.getCurrentDisplayImage();
		auto& commandBuffer = displayImage.commandBuffer;

		const auto extent = swapChain.getExtent();
		commandBuffer.beginRenderPass(renderPass, displayImage.framebuffer, extent);
	}

	void VulkanRenderer::draw(const Command& command)
	{
		auto& displayImage = rendererContext.getCurrentDisplayImage();
		auto& commandBuffer = displayImage.commandBuffer;

#	if ZINET_SANITY_CHECK
		if (!command)
		{
			Logger->error("Command is invalid");
			return;
		}
#	endif

		command(commandBuffer);
	}

	void VulkanRenderer::endRenderPass()
	{
		auto& displayImage = rendererContext.getCurrentDisplayImage();
		auto& commandBuffer = displayImage.commandBuffer;

		commandBuffer.endRenderPass();
	}

	void VulkanRenderer::endRecordingDrawCommands()
	{
		auto& displayImage = rendererContext.getCurrentDisplayImage();
		auto& commandBuffer = displayImage.commandBuffer;

		commandBuffer.end();
	}

	bool VulkanRenderer::submitCurrentDisplayImage()
	{
		auto& queue = rendererContext.queue;
		auto& currentDisplayImage = rendererContext.getCurrentDisplayImage();
		auto& fence = currentDisplayImage.fence;
		auto& commandBuffer = currentDisplayImage.commandBuffer;

		//Logger->debug("Submitting draw command buffer for image with index: {}", rendererContext.currentDisplayImageIndex);

		std::vector<VkSemaphore> waitSemaphores = { currentDisplayImage.imageAvailableSemaphore.get() };
		
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
		signalSemaphores.push_back(currentDisplayImage.renderFinishedSemaphore.get());

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

		const auto result = vkQueueSubmit(queue.get(), 1, &submitInfo, fence.get());

		if (result != VK_SUCCESS)
		{
			Logger->error("vkQueueSubmit returned not VK_SUCCESS, error code: {}", static_cast<int>(result));
			return false;
		}

		return true;
	}

	bool VulkanRenderer::displayCurrentImage()
	{
		auto& swapChain = rendererContext.swapChain;
		auto& queue = rendererContext.queue;
		auto& currentDisplayImage = rendererContext.getCurrentDisplayImage();
		auto currentDisplayImageIndex = rendererContext.currentDisplayImageIndex;

		//Logger->debug("Presenting image with index: {}", currentDisplayImageIndex);

		std::vector<VkSemaphore> waitSemaphores = { currentDisplayImage.renderFinishedSemaphore.get() };
		std::vector<VkSwapchainKHR> swapChains = { swapChain.get() };
		const VkPresentInfoKHR presentInfo
		{
			.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
			.pNext = nullptr,
			.waitSemaphoreCount = static_cast<uint32_t>(waitSemaphores.size()),
			.pWaitSemaphores = waitSemaphores.data(),
			.swapchainCount = static_cast<uint32_t>(swapChains.size()),
			.pSwapchains = swapChains.data(),
			.pImageIndices = &currentDisplayImageIndex,
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
		Logger->info("Window resized x: {}, y: {}", size.x, size.y);

		VulkanRenderer& vulkanRenderer = *(VulkanRenderer*)userPointer;
		auto& rendererContext = vulkanRenderer.getRendererContext();

		auto& device = rendererContext.device;
		auto& fence = rendererContext.getCurrentDisplayImage().fence;

		device.waitIdle();
		fence.wait(device);

		rendererContext.windowResized(size);
	}

}