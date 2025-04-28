#include "Zinet/VulkanRenderer/ZtVulkanRenderer.hpp"
#include "Zinet/VulkanRenderer/ZtShaderModule.hpp"

#include "Zinet/Window/ZtWindow.hpp"

namespace zt::vulkan_renderer
{

	bool VulkanRenderer::start(wd::Window& window) noexcept
	{
		instance.setEnableValidationLayers(true);
		if (!instance.create())
			return false;

		if (!debugUtilsMessenger.create(instance))
			return false;

		auto physicalDevices = instance.getPhysicalDevices();
		if (physicalDevices.empty())
		{
			Logger->error("Couldn't get any physical devices");
			return false;
		}

		physicalDevice = PhysicalDevice::TakeBestPhysicalDevice(physicalDevices);
		if (!physicalDevice.isValid())
			return false;

		if (!surface.create(instance, window))
			return false;

		if (!device.create(physicalDevice, surface))
			return false;

		if (!vma.create(device, physicalDevice, instance))
			return false;

		if (!swapChain.create(device, physicalDevice, surface, window))
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

		const auto renderPassCreateInfo = RenderPass::GetPresentCreateInfo(swapChain.getFormat());
		if (!renderPass.create(device, renderPassCreateInfo))
			return false;

		const auto windowSize = window.getSize();
		for (auto& imageView : imageViews)
		{
			auto& framebuffer = framebuffers.emplace_back(nullptr);
			if (!framebuffer.create(device, renderPass, imageView, windowSize))
			{
				Logger->error("Couldn't create image view from one of the swap chain images");
				return false;
			}
		}

		queue = device.getQueue();
		if (!queue.isValid())
			return false;

		if (!commandPool.create(device, queue))
			return false;

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

	void VulkanRenderer::shutdown() noexcept
	{
		device.waitIdle();

		renderPass.destroy(device);

		pipeline.destroy(device);
		pipelineLayout.destroy(device);

		imageAvailableSemaphore.destroy(device);

		renderFinishedSemaphore.destroy(device);

		fence.destroy(device);

		for (auto& framebuffer : framebuffers)
			framebuffer.destroy(device);

		for (auto& imageView : imageViews)
			imageView.destroy(device);

		commandPool.destroy(device);
		swapChain.destroy(device);
		vma.destroy();
		device.destroy();
		surface.destroy(instance);
		debugUtilsMessenger.destroy(instance);
		instance.destroy();
	}

	void VulkanRenderer::draw(const DrawInfo& drawInfo) noexcept
	{
		if (!pipeline.isValid())
		{
			GraphicsPipeline::ShadersStages shadersStages;
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

		std::uint32_t imageIndex{};
		[[maybe_unused]] const auto result = 
			vkAcquireNextImageKHR(device.get(), swapChain.get(), UINT64_MAX, imageAvailableSemaphore.get(), VK_NULL_HANDLE, &imageIndex);
		if (result != VK_SUCCESS)
		{
			Logger->error("vkAcquireNextImageKHR returned false");
		}

		vkResetCommandBuffer(commandBuffer.get(), 0);

		commandBuffer.begin();
		const auto extent = swapChain.getExtent();
		commandBuffer.beginRenderPass(renderPass, framebuffers[imageIndex], extent);

		vkCmdBindPipeline(commandBuffer.get(), VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.get());

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(swapChain.getExtent().width);
		viewport.height = static_cast<float>(swapChain.getExtent().height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		vkCmdSetViewport(commandBuffer.get(), 0, 1, &viewport);

		VkRect2D scissor{};
		scissor.offset = { 0, 0 };
		scissor.extent = swapChain.getExtent();
		vkCmdSetScissor(commandBuffer.get(), 0, 1, &scissor);

		vkCmdDraw(commandBuffer.get(), 3, 1, 0, 0);

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