#include "Zinet/GameplayLib/Systems/ZtWindowEventsSystem.hpp"

#include <ranges>

namespace zt::gameplay_lib
{
	void WindowEventsSystem::tick(float deltaTime)
	{
		tickDragable(deltaTime);
		tickClickable(deltaTime);
	}

	void WindowEventsSystem::tickDragable(float deltaTime)
	{
		if (!windowEvents)
		{
			Logger->error("WindowEvents is invalid");
			return;
		}

		for (auto& node : dragableNodes | std::views::reverse)
		{
			if (node.expired())
				continue;

			auto& mouse = windowEvents->getMouse();
			const auto mousePositionNorm = mouse.getMousePositionNorm();

			const auto viewportSize = currentCamera->getViewportRenderTarget().getResolution();
			const auto lookAt = currentCamera->getLookAt();

			// TODO: Refactor this and put it into Camera class
			const Vector2f mousePosInWorld = { (mousePositionNorm.x * viewportSize.x) - (viewportSize.x / 2.f) + lookAt.x, (mousePositionNorm.y * viewportSize.y) - (viewportSize.y / 2.f) + lookAt.y };

			const bool isPressed = mouse.isPressed(wd::MouseButton::LEFT);
			if (isPressed)
			{
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

	void WindowEventsSystem::tickClickable(float deltaTime)
	{
		if (!windowEvents)
		{
			Logger->error("WindowEvents is invalid");
			return;
		}

		auto& mouse = windowEvents->getMouse();
		const auto mousePositionNorm = mouse.getMousePositionNorm();

		const auto viewportSize = currentCamera->getViewportRenderTarget().getResolution();
		const auto lookAt = currentCamera->getLookAt();

		// TODO: Refactor this and put it into Camera class
		const Vector2f mousePosInWorld = { (mousePositionNorm.x * viewportSize.x) - (viewportSize.x / 2.f) + lookAt.x, (mousePositionNorm.y * viewportSize.y) - (viewportSize.y / 2.f) + lookAt.y };

		const bool isPressed = mouse.isPressed(wd::MouseButton::LEFT);
		if (!isPressed)
			return;

		std::int64_t nodeIndex = -1;
		for (auto& node : clickableNodes | std::views::reverse)
		{
			++nodeIndex;

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

			if (isHoveredByMouse)
				std::invoke(clickableNodesCallbacks[nodeIndex], node);
		}
	}

}