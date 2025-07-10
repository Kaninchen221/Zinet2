#include "Zinet/Gameplay/Systems/ZtSystemRenderer.hpp"
#include "Zinet/Gameplay/ZtEngineContext.hpp"

namespace zt::gameplay
{

	bool SystemRenderer::init(EngineContext& engineContext) noexcept
	{
		if (!renderer.init(engineContext.window))
			return false;

		return true;
	}

	void SystemRenderer::deinit() noexcept
	{
		const auto& device = renderer.getRendererContext().device;
		device.waitIdle();

		//imGuiIntegration.deinit(renderer.getRendererContext());
		renderer.deinit();
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