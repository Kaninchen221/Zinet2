#include "Zinet/VulkanRenderer/ZtVulkanRenderer.hpp"
#include "Zinet/VulkanRenderer/ZtShaderModule.hpp"

#include "Zinet/Window/ZtWindow.hpp"

namespace zt::vulkan_renderer
{

	bool VulkanRenderer::init(wd::Window& window) ZINET_API_POST
	{
		window.setWindowResizedCallback(this, WindowResizedCallback);

		if (!rendererContext.create(window))
			return false;

		return true;
	}

	void VulkanRenderer::deinit() ZINET_API_POST
	{
		rendererContext.device.waitIdle();

		graphicsPipeline.destroy(rendererContext);
		rendererContext.destroy();
	}

	bool VulkanRenderer::beginFrame() ZINET_API_POST
	{
		auto& swapChain = rendererContext.swapChain;
		auto& device = rendererContext.device;
		auto& fence = rendererContext.fence;
		auto& imageAvailableSemaphore = rendererContext.imageAvailableSemaphore;
		auto& currentFramebufferIndex = rendererContext.currentFramebufferIndex;

		if (!fence.wait(device))
			return false;

		if (!fence.reset(device))
			return false;

		currentFramebufferIndex = swapChain.acquireNextImage(device, imageAvailableSemaphore);
		if (currentFramebufferIndex == SwapChain::InvalidIndex)
			return false;

		return true;
	}

	bool VulkanRenderer::createPipeline(DrawInfo& drawInfo) ZINET_API_POST
	{
		if (graphicsPipeline.isValid())
			return true;

		return graphicsPipeline.create(rendererContext, drawInfo);
	}

	void VulkanRenderer::draw(const DrawInfo& drawInfo) ZINET_API_POST
	{
		graphicsPipeline.draw(rendererContext, drawInfo);
	}

	bool VulkanRenderer::submit() ZINET_API_POST
	{
		return graphicsPipeline.submit(rendererContext);
	}

	bool VulkanRenderer::endFrame() ZINET_API_POST
	{
		const auto& swapChain = rendererContext.swapChain;
		const auto& queue = rendererContext.queue;
		const auto& renderFinishedSemaphore = rendererContext.renderFinishedSemaphore;
		auto currentFramebufferIndex = rendererContext.currentFramebufferIndex;

		const VkSemaphore signalSemaphores[] = { renderFinishedSemaphore.get() };
		const VkSwapchainKHR swapChains[] = { swapChain.get() };
		const VkPresentInfoKHR presentInfo
		{
			.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
			.pNext = nullptr,
			.waitSemaphoreCount = 1,
			.pWaitSemaphores = signalSemaphores,
			.swapchainCount = 1,
			.pSwapchains = swapChains,
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
		auto& fence = rendererContext.fence;

		device.waitIdle();
		fence.wait(device);

		rendererContext.windowResized(size);

		graphicsPipeline.destroy(rendererContext);
	}

}