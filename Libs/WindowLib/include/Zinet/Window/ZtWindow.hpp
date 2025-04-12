#pragma once

#include "Zinet/Window/ZtWindowConfig.hpp"
#include "Zinet/Window/ZtGLFW.hpp"

#include "Zinet/Math/ZtVecTypes.hpp"

#include "Zinet/Core/ZtLogger.hpp"

namespace zt::wd
{
	class WindowEvents;

	class ZINET_WINDOW_LAYER_API Window
	{
	public:

		using WindowResizedCallback = std::function<void(void*, const Vector2ui&)>;

	private:

		inline static core::ConsoleLogger Logger = core::ConsoleLogger::Create("Window");

		WindowResizedCallback windowResizedCallbackPointer = nullptr;
		void* windowResizedCallbackUserPointer = nullptr;

	public:

		Window() = default;
		Window(const Window& other) = default;
		Window(Window&& other) = default;

		Window& operator = (const Window& other) = default;
		Window& operator = (Window&& other) = default;

		~Window() noexcept;

		void create(int width = 1024, int height = 1024);

		GLFWwindow* getInternal() { return internalWindow; }
		const GLFWwindow* getInternal() const { return internalWindow; }

		bool isOpen() const;

		bool shouldBeClosed() const;

		static void FramebufferSizeCallback(GLFWwindow* internalWindow, int width, int height);

		void setWindowEvents(WindowEvents* newEvent) { windowEvents = newEvent; }

		const WindowEvents& getWindowEvents() const { return *windowEvents; }
		WindowEvents& getWindowEvents() { return *windowEvents; }

		bool isMinimized() const;

		void requestCloseWindow();

		void setWindowResizedCallback(void* userPointer, WindowResizedCallback callback);

		WindowResizedCallback getWindowResizedCallback() const { return windowResizedCallbackPointer; }

		void* getWindowResizedCallbackUserPointer() { return windowResizedCallbackUserPointer; }

		void swapBuffers();

		static void SetTransparentFramebuffer(bool value);

		Vector2i getFramebufferSize() const noexcept;

		void setShowWindowBar(bool value);

		void setPosition(const Vector2i& position);
		Vector2i getPosition() const;

		void setSize(const Vector2i& size);
		Vector2i getSize() const;

		void setTitle(const std::string& title);

		void destroyWindow();

	protected:

		GLFWwindow* internalWindow{};
		WindowEvents* windowEvents{};

		void bindFramebufferSizeCallback();
	};

	inline Vector2i Window::getFramebufferSize() const noexcept
	{
		std::int32_t width, height;
		glfwGetFramebufferSize(internalWindow, &width, &height);
		return { width, height };
	}

	inline void Window::setPosition(const Vector2i& position)
	{
		if (!internalWindow)
			return;

		glfwSetWindowPos(internalWindow, position.x, position.y);
	}

	inline Vector2i Window::getPosition() const
	{
		if (!internalWindow)
			return {};

		Vector2i position;
		glfwGetWindowPos(internalWindow, &position.x, &position.y);
		return position;
	}

	inline void Window::setSize(const Vector2i& size)
	{
		if (!internalWindow)
			return;

		glfwSetWindowSize(internalWindow, size.x, size.y);
	}

	inline Vector2i Window::getSize() const
	{
		if (!internalWindow)
			return {};

		Vector2i size;
		glfwGetWindowSize(internalWindow, &size.x, &size.y);
		return size;
	}

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