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

	ZT_REFLECT_CLASS(NO_CONSTRUCTORS, NO_DESTRUCTOR, NO_OPERATORS)
	class ZINET_WINDOW_LAYER_API Keyboard : core::Object
	{
		inline static zt::core::ConsoleLogger Logger = zt::core::ConsoleLogger::Create("Keyboard");

	public:

		Keyboard() = delete;
		Keyboard(Window& newWindow);
		Keyboard(const Keyboard& other) = default;
		Keyboard(Keyboard&& other) = default;

		Keyboard& operator = (const Keyboard& other) = default;
		Keyboard& operator = (Keyboard&& other) = default;

		~Keyboard() noexcept = default;

		const Window* getWindow() const;

		const std::vector<KeyboardEvent>& getEvents() const;

		bool isPressed(KeyboardKey key) const;

		bool isReleased(KeyboardKey key) const;

		void bindCallbacks();

		void pushEvent(KeyboardKey key, std::int32_t scanCode, KeyboardEventType type, KeyboardMods mods);

		void clearEvents() { events.clear(); }

		std::string asString() const override;

	public:
/*GENERATED_CODE_START*/
/*GENERATED_CODE_END*/

	protected:

		static void KeyCallback(GLFWwindow* internalWindow, std::int32_t key, std::int32_t scanCode, std::int32_t type, std::int32_t mods);

		Window* window = nullptr;
		std::vector<KeyboardEvent> events;

	};

}