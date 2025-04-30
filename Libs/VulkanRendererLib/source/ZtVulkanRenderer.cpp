#include "Zinet/VulkanRenderer/ZtVulkanRenderer.hpp"
#include "Zinet/VulkanRenderer/ZtShaderModule.hpp"

#include "Zinet/Window/ZtWindow.hpp"

namespace zt::vulkan_renderer
{

	bool VulkanRenderer::start(wd::Window& window) noexcept
	{
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

	void VulkanRenderer::draw(const DrawInfo& drawInfo) noexcept
	{
		graphicsPipeline.draw(rendererContext, drawInfo);
	}

}