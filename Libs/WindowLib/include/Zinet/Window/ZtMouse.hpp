#pragma once

#include "Zinet/Window/ZtWindowConfig.hpp"
#include "Zinet/Window/ZtMousePositionEvent.hpp"
#include "Zinet/Window/ZtMouseButtonEvent.hpp"

#include <vector>

namespace zt::wd
{
	class Window;

	class ZINET_WINDOW_LAYER_API Mouse
	{

	public:

		Mouse() = delete;
		Mouse(Window& newWindow);
		Mouse(const Mouse& other) = default;
		Mouse(Mouse&& other) = default;

		Mouse& operator = (const Mouse& other) = default;
		Mouse& operator = (Mouse&& other) = default;

		~Mouse() noexcept = default;

		const Window* getWindow() const;

		const std::vector<MouseButtonEvent>& getButtonsEvents() const;

		void setMaxRememberedButtonsEvents(size_t size);

		size_t getMaxRememberedButtonsEvents() const;

		void bindCallbacks();

		const std::vector<MousePositionEvent>& getPositionEvents() const;

		void setMaxRememberedPositionEvents(size_t size);

		size_t getMaxRememberedPositionEvents() const;

		static void ButtonCallback(GLFWwindow* internalWindow, int button, int action, int mods);

		static void PositionCallback(GLFWwindow* internalWindow, double positionX, double positionY);

	protected:

		Window* window = nullptr;
		std::vector<MouseButtonEvent> buttonsEvents{MouseButtonEvent{}};
		std::vector<MousePositionEvent> positionEvents{MousePositionEvent{}};

	};

}