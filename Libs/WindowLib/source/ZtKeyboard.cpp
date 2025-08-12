#include "Zinet/Window/ZtKeyboard.hpp"
#include "Zinet/Window/ZtWindow.hpp"
#include "Zinet/Window/ZtWindowEvents.hpp"

namespace zt::wd
{

	Keyboard::Keyboard(Window& newWindow)
		: window{ &newWindow }
	{
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

	void Keyboard::pushEvent(KeyboardKey key, [[maybe_unused]] std::int32_t scanCode, KeyboardEventType type, KeyboardMods mods)
	{
		KeyboardEvent keyboardEvent;
		keyboardEvent.type = type;
		keyboardEvent.key = key;
		keyboardEvent.mods = mods;
		events.insert(events.begin(), keyboardEvent);
	}

	std::string Keyboard::asString() const
	{
		std::string result = "Keyboard events:\n";
		for (const auto& event : events)
		{
			result += fmt::format("Event type: {}, key: {} mods: {} \n", 
				static_cast<std::int32_t>(event.type), 
				static_cast<std::int32_t>(event.key),
				static_cast<std::int32_t>(event.mods));
		}

		return result;
	}

	void Keyboard::KeyCallback(GLFWwindow* internalWindow, std::int32_t key, std::int32_t scanCode, std::int32_t type, std::int32_t mods)
	{
		void* windowUserPointer = glfwGetWindowUserPointer(internalWindow);
		Window* window = static_cast<Window*>(windowUserPointer);
		WindowEvents& windowEvents = window->getWindowEvents();
		Keyboard& keyboard = windowEvents.getKeyboard();
		keyboard.pushEvent(
			static_cast<KeyboardKey>(key), 
			scanCode, 
			static_cast<KeyboardEventType>(type), 
			static_cast<KeyboardMods>(mods));
	}
}