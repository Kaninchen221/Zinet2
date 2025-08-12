#pragma once

#include "Zinet/Core/ZtLogger.hpp"
#include "Zinet/Core/ZtObject.hpp"

#include "Zinet/Window/ZtWindowConfig.hpp"
#include "Zinet/Window/ZtKeyboardEvent.hpp"

#include "Zinet/Core/ZtLogger.hpp"

#include <vector>

namespace zt::wd
{
	class Window;

	class Keyboard : core::Object
	{
		inline static core::ConsoleLogger Logger = core::ConsoleLogger::Create("Keyboard");

	public:

		ZINET_WINDOW_LAYER_API Keyboard() = delete;
		ZINET_WINDOW_LAYER_API Keyboard(Window& newWindow);
		ZINET_WINDOW_LAYER_API Keyboard(const Keyboard& other) = default;
		ZINET_WINDOW_LAYER_API Keyboard(Keyboard&& other) noexcept = default;

		ZINET_WINDOW_LAYER_API Keyboard& operator = (const Keyboard& other) = default;
		ZINET_WINDOW_LAYER_API Keyboard& operator = (Keyboard&& other) noexcept = default;

		ZINET_WINDOW_LAYER_API ~Keyboard() noexcept = default;

		const Window* getWindow() const noexcept { return window; }

		const std::vector<KeyboardEvent>& getEvents() const noexcept { return events; }

		ZINET_WINDOW_LAYER_API bool isPressed(KeyboardKey key) const;

		ZINET_WINDOW_LAYER_API bool isReleased(KeyboardKey key) const;

		ZINET_WINDOW_LAYER_API void bindCallbacks();

		ZINET_WINDOW_LAYER_API void pushEvent(KeyboardKey key, std::int32_t scanCode, KeyboardEventType type, KeyboardMods mods);

		void clearEvents() { events.clear(); }

		ZINET_WINDOW_LAYER_API std::string asString() const override;

	protected:

		static void KeyCallback(GLFWwindow* internalWindow, std::int32_t key, std::int32_t scanCode, std::int32_t type, std::int32_t mods);

		Window* window = nullptr;
		std::vector<KeyboardEvent> events;

	};

}