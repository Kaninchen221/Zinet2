#pragma once

#include "Zinet/Window/ZtWindowConfig.hpp"
#include "Zinet/Window/ZtKeyboard.hpp"
#include "Zinet/Window/ZtMouse.hpp"

namespace zt::wd
{
	class Window;

	class WindowEvents : public core::Object
	{
		inline static auto Logger = core::ConsoleLogger::Create("Logger");

	public:

		WindowEvents() = delete;
		WindowEvents(Window& newWindow) : window{&newWindow}, keyboard{newWindow}, mouse{newWindow} {}
		WindowEvents(const WindowEvents& other) = default;
		WindowEvents(WindowEvents&& other) noexcept = default;

		WindowEvents& operator = (const WindowEvents& other) = default;
		WindowEvents& operator = (WindowEvents&& other) noexcept = default;

		~WindowEvents() noexcept = default;

		const Window* getWindow() const noexcept { return window; }
		Window* getWindow() noexcept { return window; }

		void pollEvents();

		Keyboard& getKeyboard() noexcept { return keyboard; }
		const Keyboard& getKeyboard() const noexcept { return keyboard; }

		Mouse& getMouse() noexcept { return mouse; }
		const Mouse& getMouse() const noexcept { return mouse; }

		void bindCallbacks();

		std::string asString() const override;

	protected:

		Window* window = nullptr;
		Keyboard keyboard;
		Mouse mouse;

	};

	inline void WindowEvents::bindCallbacks()
	{
		keyboard.bindCallbacks();
		mouse.bindCallbacks();
	}

	inline void WindowEvents::pollEvents()
	{
		keyboard.clearEvents();
		mouse.clearEvents();
		glfwPollEvents();
	}

	inline std::string WindowEvents::asString() const
	{
		return fmt::format("Window events:\n{}{}", keyboard.asString(), mouse.asString());
	}
}