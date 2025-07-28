#pragma once

#include "Zinet/Window/ZtWindowConfig.hpp"
#include "Zinet/Window/ZtKeyboard.hpp"
#include "Zinet/Window/ZtMouse.hpp"

namespace zt::wd
{
	class Window;

	ZT_REFLECT_CLASS(NO_CONSTRUCTORS, NO_DESTRUCTOR, NO_OPERATORS)
	class ZINET_WINDOW_LAYER_API WindowEvents : public core::Object
	{
		inline static auto Logger = core::ConsoleLogger::Create("Logger");

	public:

		WindowEvents() = delete;
		WindowEvents(Window& newWindow) : core::Object{ "WindowEvents" }, window{&newWindow}, keyboard{newWindow}, mouse{newWindow} {}
		WindowEvents(const WindowEvents& other) = default;
		WindowEvents(WindowEvents&& other) = default;

		WindowEvents& operator = (const WindowEvents& other) = default;
		WindowEvents& operator = (WindowEvents&& other) = default;

		~WindowEvents() ZINET_API_POST = default;

		const Window* getWindow() const { return window; }
		Window* getWindow() { return window; }

		void pollEvents();

		Keyboard& getKeyboard() { return keyboard; }
		const Keyboard& getKeyboard() const { return keyboard; }

		Mouse& getMouse() { return mouse; }
		const Mouse& getMouse() const { return mouse; }

		void bindCallbacks();

		std::string asString() const override;

	protected:

		Window* window = nullptr;
		Keyboard keyboard;
		Mouse mouse;

	};

}