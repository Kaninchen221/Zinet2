#include "Zinet/Window/ZtMouse.hpp"
#include "Zinet/Window/ZtWindow.hpp"
#include "Zinet/Window/ZtMouseButtonEventType.hpp"
#include "Zinet/Window/ZtMouseButton.hpp"
#include "Zinet/Window/ZtMouseButtonEvent.hpp"

namespace zt::wd
{
	Mouse::Mouse(Window& newWindow)
		: window{ &newWindow }
	{

	}

	const Window* Mouse::getWindow() const
	{
		return window;
	}

	const std::vector<MouseButtonEvent>& Mouse::getButtonsEvents() const
	{
		return buttonsEvents;
	}

	void Mouse::setMaxRememberedButtonsEvents(size_t size)
	{
		buttonsEvents.resize(size);
	}

	size_t Mouse::getMaxRememberedButtonsEvents() const
	{
		return buttonsEvents.size();
	}

	void Mouse::ButtonCallback(GLFWwindow* glfwWindow, int button, int action, int mods)
	{
		void* windowUserPointer = glfwGetWindowUserPointer(glfwWindow);
		Window* window = static_cast<Window*>(windowUserPointer);
		Event& event = window->getEvent();
		Mouse& mouse = event.getMouse();

		mouse.buttonsEvents.pop_back();

		MouseButtonEvent buttonEvent;
		buttonEvent.type = static_cast<MouseButtonEventType>(action);
		buttonEvent.button = static_cast<MouseButton>(button);
		mouse.buttonsEvents.insert(mouse.buttonsEvents.begin(), buttonEvent);
	}

	void Mouse::bindCallbacks()
	{
		glfwSetMouseButtonCallback(window->getInternal(), &Mouse::ButtonCallback);
		glfwSetCursorPosCallback(window->getInternal(), &Mouse::PositionCallback);
	}

	void Mouse::PositionCallback(GLFWwindow* glfwWindow, double positionX, double positionY)
	{
		Window* windowUserPointer = static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));
		Event& event = windowUserPointer->getEvent();
		Mouse& mouse = event.getMouse();

		mouse.positionEvents.pop_back();

		MousePositionEvent positionEvent{};
		positionEvent.position = Vector2d(positionX, positionY);
		mouse.positionEvents.insert(mouse.positionEvents.begin(), positionEvent);
	}

	const std::vector<MousePositionEvent>& Mouse::getPositionEvents() const
	{
		return positionEvents;
	}

	void Mouse::setMaxRememberedPositionEvents(size_t size)
	{
		positionEvents.resize(size);
	}

	size_t Mouse::getMaxRememberedPositionEvents() const
	{
		return positionEvents.size();
	}

}