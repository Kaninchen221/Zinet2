#include "Zinet/VulkanRenderer/ZtVulkanRenderer.hpp"
#include "Zinet/VulkanRenderer/ZtShaderModule.hpp"

#include "Zinet/Window/ZtWindow.hpp"

namespace zt::vulkan_renderer
{

	bool VulkanRenderer::start(wd::Window& window) noexcept
	{
		window.setWindowResizedCallback(this, WindowResizedCallback);

		if (!rendererContext.create(window))
			return false;

		if (!graphicsPipeline.create(rendererContext))
			return false;

		return true;
	}

	void VulkanRenderer::shutdown() noexcept
	{
		rendererContext.device.waitIdle();

		graphicsPipeline.destroy(rendererContext);
		rendererContext.destroy();
	}

	bool VulkanRenderer::preDraw(const DrawInfo& drawInfo) noexcept
	{
		return graphicsPipeline.preDraw(rendererContext, drawInfo);
	}

	void VulkanRenderer::draw(const DrawInfo& drawInfo) noexcept
	{
		graphicsPipeline.draw(rendererContext, drawInfo);
	}

	void VulkanRenderer::WindowResizedCallback(void* userPointer, const Vector2i& size)
	{
		VulkanRenderer& vulkanRenderer = *(VulkanRenderer*)userPointer;
		auto& rendererContext = vulkanRenderer.getRendererContext();

		auto& swapChain = rendererContext.swapChain;
		auto& device = rendererContext.device;
		auto& physicalDevice = rendererContext.physicalDevice;
		auto& surface = rendererContext.surface;

		auto& graphicsPipeline = vulkanRenderer.getGraphicsPipeline();

		device.waitIdle();
		graphicsPipeline.fence.wait(device);

		graphicsPipeline.destroy(rendererContext);

		swapChain.destroy(device);

		swapChain.create(device, physicalDevice, surface, size);

		graphicsPipeline.create(rendererContext);
	}

}