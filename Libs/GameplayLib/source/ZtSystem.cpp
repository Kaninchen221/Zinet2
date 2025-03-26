#include "Zinet/GameplayLib/ZtSystem.hpp"

#include <ranges>

namespace zt::gameplay_lib
{

	void TickableSystem::tick(float deltaTime)
	{
		for (const auto& node : nodes)
		{
			if (node.expired())
				continue;

			auto asShared = node.lock();
			asShared->tick(deltaTime);
		}
	}

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

	void DragableSystem::tick(float deltaTime)
	{
		if (!event)
		{
			Logger->error("Event is invalid");
			return;
		}

		for (auto& node : nodes | std::views::reverse)
		{
			if (node.expired())
				continue;

			auto& mouse = event->getMouse();
			const auto mousePositionNorm = mouse.getMousePositionNorm();

			const auto viewportSize = currentCamera->getViewportRenderTarget().getResolution();
			const auto lookAt = currentCamera->getLookAt();

			// TODO: Refactor this and put it into Camera class
			const Vector2f mousePosInWorld = { (mousePositionNorm.x * viewportSize.x) - (viewportSize.x / 2.f) + lookAt.x, (mousePositionNorm.y * viewportSize.y) - (viewportSize.y / 2.f) + lookAt.y };

			const bool isPressed = mouse.isPressed(wd::MouseButton::LEFT);
			if (isPressed)
			{
				if (node.expired())
					continue;

				auto asShared = node.lock();

				bool isHoveredByMouse = false;
				{
					const auto nodePos = asShared->getAbsolutePosition();
					const auto nodeMin = nodePos;
					const auto nodeMax = nodePos + asShared->getSize();
					// TODO: Refactor this to AABB func
					if (mousePosInWorld.x > nodeMin.x && mousePosInWorld.x < nodeMax.x && mousePosInWorld.y > nodeMin.y && mousePosInWorld.y < nodeMax.y)
						isHoveredByMouse = true;
				}

				if (dragedNode.expired() && isHoveredByMouse)
				{
					dragedNode = node;
					offset = mousePosInWorld - asShared->getPosition();
				}

				auto dragedNodeAsShared = dragedNode.lock();
				if (dragedNodeAsShared == asShared)
				{
					const Vector2f newPosition = mousePosInWorld - offset;
					asShared->setPosition(newPosition);
				}
			}
			else
			{
				dragedNode.reset();
			}
		}
	}

}