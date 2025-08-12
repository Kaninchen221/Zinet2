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
		ZINET_WINDOW_LAYER_API Mouse(const Mouse& other) = default;
		ZINET_WINDOW_LAYER_API Mouse(Mouse&& other) noexcept = default;

		ZINET_WINDOW_LAYER_API Mouse& operator = (const Mouse& other) = default;
		ZINET_WINDOW_LAYER_API Mouse& operator = (Mouse&& other) noexcept = default;

		ZINET_WINDOW_LAYER_API ~Mouse() noexcept = default;

		const Window* getWindow() const noexcept { return window; }
		Window* getWindow() noexcept { return window; }

		ZINET_WINDOW_LAYER_API bool isPressed(MouseButton mouseButton) const;

		ZINET_WINDOW_LAYER_API bool isReleased(MouseButton mouseButton) const;

		Vector2d getMousePosition() const noexcept;

		Vector2d getMousePositionNorm() const;

		const std::vector<MouseButtonEvent>& getButtonsEvents() const noexcept { return buttonsEvents; }

		const std::vector<MousePositionEvent>& getPositionEvents() const noexcept { return positionEvents; }

		ZINET_WINDOW_LAYER_API void bindCallbacks();

		ZINET_WINDOW_LAYER_API static void ButtonCallback(GLFWwindow* internalWindow, int button, int action, int mods);

		ZINET_WINDOW_LAYER_API void pushButtonEvent(int button, int action, int mods);

		ZINET_WINDOW_LAYER_API static void PositionCallback(GLFWwindow* internalWindow, double positionX, double positionY);

		ZINET_WINDOW_LAYER_API void pushPositionEvent(double positionX, double positionY);

		ZINET_WINDOW_LAYER_API void clearEvents();

		ZINET_WINDOW_LAYER_API std::string asString() const;

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