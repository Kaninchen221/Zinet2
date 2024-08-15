#pragma once

#include "Zinet/Window/ZtWindowConfig.hpp"
#include "Zinet/Window/ZtEvent.hpp"
#include "Zinet/Window/ZtGLFW.hpp"

#include "Zinet/Math/ZtVecTypes.hpp"

#include "Zinet/Core/ZtLogger.hpp"

namespace zt::wd
{
	class ZINET_WINDOW_LAYER_API Window
	{
	public:

		using WindowResizedCallback = std::function<void(void*, const Vector2ui&)>;

	private:

		inline static core::ConsoleLogger Logger = core::ConsoleLogger::Create("Window");

		WindowResizedCallback windowResizedCallbackPointer = nullptr;
		void* windowResizedCallbackUserPointer = nullptr;

	public:

		Window() : event{ *this } {}
		Window(const Window& other) = default;
		Window(Window&& other) = default;

		Window& operator = (const Window& other) = default;
		Window& operator = (Window&& other) = default;

		~Window() noexcept;

		void create();

		GLFWwindow* getInternal();

		const GLFWwindow* getInternal() const;

		void bindCallbacks();

		bool isOpen() const;

		bool shouldBeClosed() const;

		static void FramebufferSizeCallback(GLFWwindow* internalWindow, int width, int height);

		const Event& getEvent() const { return event; }
		Event& getEvent() { return event; }

		Vector2ui getSize() const;

		bool isMinimized() const;

		void requestCloseWindow();

		void setWindowResizedCallback(void* userPointer, WindowResizedCallback callback);

		WindowResizedCallback getWindowResizedCallback() const { return windowResizedCallbackPointer; }

		void* getWindowResizedCallbackUserPointer() { return windowResizedCallbackUserPointer; }

	protected:

		GLFWwindow* internalWindow{};
		Event event;

		void bindFramebufferSizeCallback();
	};

	inline void Window::setWindowResizedCallback(void* userPointer, WindowResizedCallback callback)
	{
		if (userPointer && callback)
		{
			windowResizedCallbackPointer = callback;
			windowResizedCallbackUserPointer = userPointer;
		}
		else
		{
			Logger->error("Can't bind user pointer: {} : Is callback valid: {}", userPointer, callback.operator bool());
		}
	}
}