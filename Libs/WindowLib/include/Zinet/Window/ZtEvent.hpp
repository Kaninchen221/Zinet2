#pragma once

#include "Zinet/Window/ZtWindowConfig.hpp"
#include "Zinet/Window/ZtKeyboard.hpp"
#include "Zinet/Window/ZtMouse.hpp"

namespace zt::wd
{
	class Window;

	ZT_REFLECT_CLASS(NO_CONSTRUCTORS, NO_DESTRUCTOR, NO_OPERATORS)
	class ZINET_WINDOW_LAYER_API Event : public core::Object
	{
		inline static auto Logger = core::ConsoleLogger::Create("Logger");

	public:

		Event() = delete;
		Event(Window& newWindow) : window{ &newWindow }, keyboard{ newWindow }, mouse{ newWindow } {}
		Event(const Event& other) = default;
		Event(Event&& other) = default;

		Event& operator = (const Event& other) = default;
		Event& operator = (Event&& other) = default;

		~Event() noexcept = default;

		const Window* getWindow() const;

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