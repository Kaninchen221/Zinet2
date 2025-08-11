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

	class ZINET_WINDOW_LAYER_API Keyboard : core::Object
	{
		inline static core::ConsoleLogger Logger = core::ConsoleLogger::Create("Keyboard");

	public:

		Keyboard() = delete;
		Keyboard(Window& newWindow);
		Keyboard(const Keyboard& other) = default;
		Keyboard(Keyboard&& other) noexcept = default;

		Keyboard& operator = (const Keyboard& other) = default;
		Keyboard& operator = (Keyboard&& other) noexcept = default;

		~Keyboard() noexcept = default;

		const Window* getWindow() const noexcept;

		const std::vector<KeyboardEvent>& getEvents() const noexcept;

		bool isPressed(KeyboardKey key) const;

		bool isReleased(KeyboardKey key) const;

		void bindCallbacks();

		void pushEvent(KeyboardKey key, std::int32_t scanCode, KeyboardEventType type, KeyboardMods mods);

		void clearEvents() { data.events.clear(); }

		std::string asString() const override;

	protected:

		static void KeyCallback(GLFWwindow* internalWindow, std::int32_t key, std::int32_t scanCode, std::int32_t type, std::int32_t mods);

		struct Data
		{
			Window* window = nullptr;
			std::vector<KeyboardEvent> events;
		};
		Data data;

	};

}