#include "Zinet/Window/ZtKeyboard.hpp"
#include "Zinet/Window/ZtWindow.hpp"

namespace zt::wd
{

	Keyboard::Keyboard(Window& newWindow)
		: events{ KeyboardEvent{} },
		window{ &newWindow }
	{
	}

	const Window* Keyboard::getWindow() const
	{
		return window;
	}

	const std::vector<KeyboardEvent>& Keyboard::getEvents() const
	{
		return events;
	}

	bool Keyboard::isPressed(KeyboardKey key) const
	{
		return glfwGetKey(window->getInternal(), static_cast<int>(key)) == static_cast<int>(KeyboardEventType::Pressed);
	}

	bool Keyboard::isReleased(KeyboardKey key) const
	{
		return glfwGetKey(window->getInternal(), static_cast<int>(key)) == static_cast<int>(KeyboardEventType::Released);
	}

	void Keyboard::bindCallbacks()
	{
		if (window == nullptr)
		{
			Logger->error("Can't bind callbacks when window is nullptr");
			return;
		}

		glfwSetKeyCallback(window->getInternal(), Keyboard::KeyCallback);
	}

	void Keyboard::KeyCallback(GLFWwindow* internalWindow, int key, int scanCode, int action, int mods)
	{
		void* windowUserPointer = glfwGetWindowUserPointer(internalWindow);
		Window* window = static_cast<Window*>(windowUserPointer);
		Event& event = window->getEvent();
		Keyboard& keyboard = event.getKeyboard();

		keyboard.events.pop_back();

		KeyboardEvent keyboardEvent;
		keyboardEvent.type = static_cast<KeyboardEventType>(action);
		keyboardEvent.key = static_cast<KeyboardKey>(key);
		keyboard.events.insert(keyboard.events.begin(), keyboardEvent);
	}

	void Keyboard::setMaximumRememberedEvents(size_t value)
	{
		events.resize(value);
	}

	size_t Keyboard::getMaximumRememberedEvents() const
	{
		return events.size();
	}

}