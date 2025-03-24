#pragma once

#include "Zinet/Window/ZtWindowConfig.hpp"
#include "Zinet/Window/ZtMousePositionEvent.hpp"
#include "Zinet/Window/ZtMouseButtonEvent.hpp"
#include "Zinet/Window/ZtWindow.hpp"

#include <vector>

#include "Zinet/Core/ZtObject.hpp"

namespace zt::wd
{
	ZT_REFLECT_CLASS(NO_CONSTRUCTORS, NO_DESTRUCTOR, NO_OPERATORS, NO_CREATE_COPY, NO_REGISTER_CLASS)
	class ZINET_WINDOW_LAYER_API Mouse : public core::Object
	{

	public:

		Mouse() = delete;
		Mouse(Window& newWindow);
		Mouse(const Mouse& other) = default;
		Mouse(Mouse&& other) = default;

		Mouse& operator = (const Mouse& other) = default;
		Mouse& operator = (Mouse&& other) = default;

		~Mouse() noexcept = default;

		const Window* getWindow() const { return window; }
		Window* getWindow() { return window; }

		bool isPressed(MouseButton mouseButton) const;

		bool isReleased(MouseButton mouseButton) const;

		Vector2d getMousePosition() const;

		Vector2d getMousePositionNorm() const;

		const std::vector<MouseButtonEvent>& getButtonsEvents() const { return buttonsEvents; }

		const std::vector<MousePositionEvent>& getPositionEvents() const { return positionEvents; }

		void bindCallbacks();

		static void ButtonCallback(GLFWwindow* internalWindow, int button, int action, int mods);

		void pushButtonEvent(int button, int action, int mods);

		static void PositionCallback(GLFWwindow* internalWindow, double positionX, double positionY);

		void pushPositionEvent(double positionX, double positionY);

		void clearEvents();

		std::string asString() const override;

	public:
/*GENERATED_CODE_START*/
		static_assert(IsObjectClassInherited); // Class using ZT_REFLECT_CLASS should inherit public from Object class
		
		class ClassInfo : public zt::core::ClassInfoBase
		{
		public:
		
			std::string_view getClassName() const override { return "Mouse"; }
		};
		const zt::core::ClassInfoBase* getClassInfo() const override { static ClassInfo classInfo; return &classInfo; }
		
		
/*GENERATED_CODE_END*/

	protected:

		Window* window = nullptr;
		std::vector<MouseButtonEvent> buttonsEvents;
		std::vector<MousePositionEvent> positionEvents;

	};

	inline Vector2d Mouse::getMousePosition() const
	{
		Vector2d position;
		glfwGetCursorPos(window->getInternal(), &position.x, &position.y);
		return position;
	}

	inline Vector2d Mouse::getMousePositionNorm() const
	{
		const auto mousePosition = getMousePosition();
		const auto windowSize = window->getSize();
		return
		{
			mousePosition.x / windowSize.x,
			mousePosition.y / windowSize.y
		};
	}
}