#include "Zinet/Gameplay/Systems/ZtSystemRenderer.hpp"

namespace zt::gameplay
{

	bool SystemRenderer::init() noexcept
	{
		wd::GLFW::Init(false);

		//wd::Window::SetTransparentFramebuffer(true);

		if (!window.create())
			return false;

		//window.makeWindowTransparentWhileUsingVulkan();

		if (!renderer.init(window))
			return false;

		return true;
	}

	bool SystemRenderer::deinit() noexcept
	{
		const auto& device = renderer.getRendererContext().device;
		device.waitIdle();

		//imGuiIntegration.deinit(renderer.getRendererContext());
		renderer.deinit();
		window.destroyWindow();
		wd::GLFW::Deinit();

		return true;
	}

	void SystemRenderer::addNode(const NodeWeakHandle<NodeT>& node) noexcept
	{
		ClassBaseT::addNode(node);

		drawInfo.instances = static_cast<uint32_t>(nodes.size());
	}

	void SystemRenderer::update() noexcept
	{
		if (drawInfo.instances == 0)
			return;

		renderer.createPipeline(drawInfo);
		renderer.getGraphicsPipeline().isValid();

		renderer.beginFrame();

		renderer.draw(drawInfo);

		renderer.submit();

		renderer.endFrame();
	}

}