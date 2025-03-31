#include "Zinet/GameplayLib/Systems/ZtWindowEventsSystem.hpp"

#include "Zinet/Math/ZtMath.hpp"

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

			const Vector2f mousePosInWorld = currentCamera->mousePositionNormToWorld(mousePositionNorm);

			const bool isPressed = mouse.isPressed(wd::MouseButton::LEFT);
			if (isPressed)
			{
				auto asShared = node.lock();

				bool isHoveredByMouse = false;
				{
					const auto nodePos = asShared->getAbsolutePosition();
					isHoveredByMouse = Math::IsInsideRect({ nodePos, asShared->getSize() }, mousePosInWorld);
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

		if (!dragedNode.expired())
			return;

		auto& mouse = windowEvents->getMouse();
		const auto mousePositionNorm = mouse.getMousePositionNorm();

		const Vector2f mousePosInWorld = currentCamera->mousePositionNormToWorld(mousePositionNorm);

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
				isHoveredByMouse = Math::IsInsideRect({ nodePos, asShared->getSize() }, mousePosInWorld);
			}

			if (isHoveredByMouse)
				std::invoke(clickableNodesCallbacks[nodeIndex], node);
		}
	}

}