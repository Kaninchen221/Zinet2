#include "Zinet/Window/ZtMouse.hpp"
#include "Zinet/Window/ZtWindow.hpp"
#include "Zinet/Window/ZtMouseButtonEventType.hpp"
#include "Zinet/Window/ZtMouseButton.hpp"
#include "Zinet/Window/ZtMouseButtonEvent.hpp"
#include "Zinet/Window/ZtWindowEvents.hpp"

namespace zt::wd
{
	Mouse::Mouse(Window& newWindow)
		: window{ &newWindow }
	{

	}

	bool Mouse::isPressed(MouseButton mouseButton) const
	{
		return glfwGetMouseButton(window->getInternal(), static_cast<int>(mouseButton)) == static_cast<int>(MouseButtonEventType::Pressed);
	}

	bool Mouse::isReleased(MouseButton mouseButton) const
	{
		return glfwGetMouseButton(window->getInternal(), static_cast<int>(mouseButton)) == static_cast<int>(MouseButtonEventType::Released);
	}

	void Mouse::ButtonCallback(GLFWwindow* glfwWindow, int button, int action, int mods)
	{
		void* windowUserPointer = glfwGetWindowUserPointer(glfwWindow);
		Window* window = static_cast<Window*>(windowUserPointer);
		WindowEvents& windowEvents = window->getWindowEvents();
		Mouse& mouse = windowEvents.getMouse();
		mouse.pushButtonEvent(button, action, mods);
	}

	void Mouse::bindCallbacks()
	{
		glfwSetMouseButtonCallback(window->getInternal(), &Mouse::ButtonCallback);
		glfwSetCursorPosCallback(window->getInternal(), &Mouse::PositionCallback);
	}

	void Mouse::PositionCallback(GLFWwindow* glfwWindow, double positionX, double positionY)
	{
		Window* windowUserPointer = static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));
		WindowEvents& windowEvents = windowUserPointer->getWindowEvents();
		Mouse& mouse = windowEvents.getMouse();
		mouse.pushPositionEvent(positionX, positionY);
	}

	void Mouse::pushPositionEvent(double positionX, double positionY)
	{
		MousePositionEvent positionEvent{};
		positionEvent.position.x = positionX;
		positionEvent.position.y = positionY;
		positionEvents.insert(positionEvents.begin(), positionEvent);
	}

	void Mouse::pushButtonEvent(int button, int action, [[maybe_unused]] int mods)
	{
		MouseButtonEvent buttonEvent;
		buttonEvent.type = static_cast<MouseButtonEventType>(action);
		buttonEvent.button = static_cast<MouseButton>(button);
		buttonsEvents.insert(buttonsEvents.begin(), buttonEvent);
	}

	void Mouse::clearEvents()
	{
		buttonsEvents.clear();
		positionEvents.clear();
	}

	std::string Mouse::asString() const
	{
		std::string result = "Mouse events:\n";
		result += "Button events:\n";
		for (const auto& buttonEvent : buttonsEvents)
		{
			result += fmt::format("Event type: {}, key: {}\n",
				static_cast<std::int32_t>(buttonEvent.type),
				static_cast<std::int32_t>(buttonEvent.button));
		}

		result += "\nMouse events:\n";
		for (const auto& positionEvent : positionEvents)
		{
			result += fmt::format("Position event x: {}, y: {}\n",
				positionEvent.position.x,
				positionEvent.position.y);
		}

		return result;
	}

}