#include "Zinet/GameplayLib/Systems/ZtDrawableSystem.hpp"

#include <ranges>

namespace zt::gameplay_lib
{
	void DrawableSystem::tick(float deltaTime)
	{
		auto& viewportRenderTarget = currentCamera->getViewportRenderTarget();
		viewportRenderTarget.fill(sf::ZeroColor);

		auto drawInfoIt = nodesDrawInfos.end();
		for (const auto& node : nodes | std::views::reverse)
		{
			--drawInfoIt;

			if (node.expired())
				continue;

			auto& drawInfo = *drawInfoIt;

			auto asShared = node.lock();
			if (asShared->isDrawInfoDirty() || drawInfo.isDirty)
			{
				drawInfo = asShared->getDrawInfo();
				auto& vertexShader = drawInfo.shaderProgram.vertexShader;
				vertexShader.cameraPosition = currentCamera->getPosition();
				vertexShader.cameraSize = currentCamera->getViewportRenderTarget().getResolution();
				softwareRenderer.draw(drawInfo, viewportRenderTarget);
			}
			else
			{
				softwareRenderer.draw(drawInfo, viewportRenderTarget);
			}
		}
	}

	void DrawableSystem::addNode(const std::weak_ptr<Node>& node)
	{
		if (node.expired())
			return; // TODO: Add node should return bool

		nodes.push_back(node);

		auto nodeAsShared = node.lock();
		nodesDrawInfos.push_back(nodeAsShared->getDrawInfo());
	}

}