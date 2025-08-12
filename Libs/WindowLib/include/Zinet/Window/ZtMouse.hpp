#pragma once

#include "Zinet/Window/ZtWindowConfig.hpp"
#include "Zinet/Window/ZtMousePositionEvent.hpp"
#include "Zinet/Window/ZtMouseButtonEvent.hpp"
#include "Zinet/Window/ZtWindow.hpp"

#include <vector>

#include "Zinet/Core/ZtObject.hpp"

namespace zt::wd
{
	class Mouse
	{

	public:

		Mouse() = delete;
		Mouse(Window& newWindow) : window{ &newWindow } {}
		 Mouse(const Mouse& other) = default;
		 Mouse(Mouse&& other) noexcept = default;

		 Mouse& operator = (const Mouse& other) = default;
		 Mouse& operator = (Mouse&& other) noexcept = default;

		 ~Mouse() noexcept = default;

		const Window* getWindow() const noexcept { return window; }
		Window* getWindow() noexcept { return window; }

		 bool isPressed(MouseButton mouseButton) const;

		 bool isReleased(MouseButton mouseButton) const;

		Vector2d getMousePosition() const noexcept;

		Vector2d getMousePositionNorm() const;

		const std::vector<MouseButtonEvent>& getButtonsEvents() const noexcept { return buttonsEvents; }

		const std::vector<MousePositionEvent>& getPositionEvents() const noexcept { return positionEvents; }

		 void bindCallbacks();

		 static void ButtonCallback(GLFWwindow* internalWindow, int button, int action, int mods);

		 void pushButtonEvent(int button, int action, int mods);

		 static void PositionCallback(GLFWwindow* internalWindow, double positionX, double positionY);

		 void pushPositionEvent(double positionX, double positionY);

		 void clearEvents();

		 std::string asString() const;

	protected:

		Window* window = nullptr;
		std::vector<MouseButtonEvent> buttonsEvents;
		std::vector<MousePositionEvent> positionEvents;

	};

	inline Vector2d Mouse::getMousePosition() const noexcept
	{
		Vector2d position;
		glfwGetCursorPos(window->getInternal(), &position.x, &position.y);
		return position;
	}

	inline Vector2d Mouse::getMousePositionNorm() const
	{
		const auto mousePosition = getMousePosition();
		const auto windowSize = window->getSize();
		return
		{
			mousePosition.x / windowSize.x,
			mousePosition.y / windowSize.y
		};
	}
}