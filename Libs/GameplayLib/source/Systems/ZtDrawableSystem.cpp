#include "Zinet/GameplayLib/Systems/ZtDrawableSystem.hpp"

#include <ranges>

namespace zt::gameplay_lib
{
	void DrawableSystem::tick(float deltaTime)
	{
		auto& viewportRenderTarget = currentCamera->getViewportRenderTarget();
		viewportRenderTarget.fill(sf::ZeroColor);

		for (const auto& node : nodes)
		{
			if (node.expired())
				continue;

			auto asShared = node.lock();
			auto drawInfo = asShared->getDrawInfo();
			auto& vertexShader = drawInfo.shaderProgram.vertexShader;
			vertexShader.cameraPosition = currentCamera->getPosition();
			vertexShader.cameraSize = currentCamera->getViewportRenderTarget().getResolution();
			softwareRenderer.draw(drawInfo, viewportRenderTarget);
		}
	}
}