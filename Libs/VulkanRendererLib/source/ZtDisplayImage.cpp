#include "Zinet/VulkanRenderer/ZtDisplayImage.hpp"
#include "Zinet/VulkanRenderer/ZtRendererContext.hpp"
#include "Zinet/VulkanRenderer/ZtTexture.hpp"
#include "Zinet/VulkanRenderer/ZtSampler.hpp"

namespace zt::vulkan_renderer
{
	bool DisplayImage::create(RendererContext& rendererContext, VkImage swapChainImage, uint32_t index)
	{
		auto& swapChain = rendererContext.getSwapChain();
		auto& device = rendererContext.getDevice();
		auto& renderPass = rendererContext.getRenderPass();
		auto& commandPool = rendererContext.getCommandPool();

		image = swapChainImage;

		const auto imageViewCreateInfo = ImageView::GetDefaultCreateInfo(image, swapChain.getFormat());
		if (!imageView.create(device, imageViewCreateInfo))
		{
			Logger->error("Couldn't create an image view from one of the swap chain images");
			return false;
		}
		device.setDebugName(imageView, fmt::format("DisplayImage ImageView {}", index).c_str(), VK_OBJECT_TYPE_IMAGE_VIEW);

		const auto swapChainSize = swapChain.getExtent();
		const Vector2ui framebufferSize{ swapChainSize.width, swapChainSize.height };
		if (!framebuffer.create(device, renderPass, imageView, framebufferSize))
		{
			Logger->error("Couldn't create framebuffer from one of the swap chain images");
			return false;
		}
		device.setDebugName(framebuffer, fmt::format("DisplayImage Framebuffer {}", index).c_str(), VK_OBJECT_TYPE_FRAMEBUFFER);

		if (!commandBuffer.create(device, commandPool))
			return false;

		device.setDebugName(commandBuffer, fmt::format("DisplayImage CommandBuffer {}", index).c_str(), VK_OBJECT_TYPE_COMMAND_BUFFER);

		if (!fence.create(device, true))
			return false;

		device.setDebugName(fence, fmt::format("DisplayImage Fence {}", index).c_str(), VK_OBJECT_TYPE_FENCE);

		if (!imageAvailableSemaphore.create(device))
			return false;

		device.setDebugName(imageAvailableSemaphore, fmt::format("DisplayImage Image Available Semaphore {}", index).c_str(), VK_OBJECT_TYPE_SEMAPHORE);

		if (!renderFinishedSemaphore.create(device))
			return false;

		device.setDebugName(renderFinishedSemaphore, fmt::format("DisplayImage Render Finished Semaphore {}", index).c_str(), VK_OBJECT_TYPE_SEMAPHORE);

		return true;
	}

	void DisplayImage::destroy(RendererContext& rendererContext)
	{
		auto& device = rendererContext.getDevice();

		renderFinishedSemaphore.destroy(device);
		imageAvailableSemaphore.destroy(device);
		fence.destroy(device);
		image = nullptr;
		framebuffer.destroy(device);
		imageView.destroy(device);
		commandBuffer.invalidate();
	}
}
