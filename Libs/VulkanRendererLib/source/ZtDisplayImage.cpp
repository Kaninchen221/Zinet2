#include "Zinet/VulkanRenderer/ZtDisplayImage.hpp"
#include "Zinet/VulkanRenderer/ZtRendererContext.hpp"
#include "Zinet/VulkanRenderer/ZtTexture.hpp"
#include "Zinet/VulkanRenderer/ZtSampler.hpp"

namespace zt::vulkan_renderer
{
	bool DisplayImage::create(RendererContext& rendererContext, VkImage swapChainImage)
	{
		auto& swapChain = rendererContext.swapChain;
		auto& device = rendererContext.device;
		auto& renderPass = rendererContext.renderPass;
		auto& commandPool = rendererContext.commandPool;

		image = swapChainImage;

		const auto imageViewCreateInfo = ImageView::GetDefaultCreateInfo(image, swapChain.getFormat());
		if (!imageView.create(device, imageViewCreateInfo))
		{
			Logger->error("Couldn't create an image view from one of the swap chain images");
			return false;
		}

		const auto swapChainSize = swapChain.getExtent();
		const Vector2ui framebufferSize{ swapChainSize.width, swapChainSize.height };
		if (!framebuffer.create(device, renderPass, imageView, framebufferSize))
		{
			Logger->error("Couldn't create framebuffer from one of the swap chain images");
			return false;
		}

		if (!fence.create(device, true))
			return false;

		if (!commandBuffer.create(device, commandPool))
			return false;

		return true;
	}

	void DisplayImage::destroy(RendererContext& rendererContext)
	{
		auto& device = rendererContext.device;

		image = nullptr;
		framebuffer.destroy(device);
		imageView.destroy(device);
		fence.destroy(device);
		commandBuffer.invalidate();
	}
}
