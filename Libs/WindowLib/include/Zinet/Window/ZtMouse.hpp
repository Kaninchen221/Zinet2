#pragma once

#include "Zinet/Window/ZtWindowConfig.hpp"
#include "Zinet/Window/ZtMousePositionEvent.hpp"
#include "Zinet/Window/ZtMouseButtonEvent.hpp"

#include <vector>

#include "Zinet/Core/ZtObject.hpp"

namespace zt::wd
{
	class Window;

	ZT_REFLECT_CLASS(NO_CONSTRUCTORS, NO_DESTRUCTOR, NO_OPERATORS)
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
/*GENERATED_CODE_END*/

	protected:

		Window* window = nullptr;
		std::vector<MouseButtonEvent> buttonsEvents;
		std::vector<MousePositionEvent> positionEvents;

	};

}