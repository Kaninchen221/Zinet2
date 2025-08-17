#pragma once

#include "Zinet/Window/ZtWindowConfig.hpp"
#include "Zinet/Window/ZtGLFW.hpp"

#include "Zinet/Math/ZtVecTypes.hpp"

#include "Zinet/Core/ZtLogger.hpp"

namespace zt::wd
{
	class WindowEvents;

	class Window
	{
	public:

		using WindowResizedCallback = void (*)(void*, const Vector2i&);

	private:

		inline static core::ConsoleLogger Logger = core::ConsoleLogger::Create("Window");

		WindowResizedCallback windowResizedCallback = nullptr;
		void* windowResizedCallbackUserPointer = nullptr;

	public:

		Window() noexcept = default;
		Window(const Window& other) noexcept = default;
		Window(Window&& other) noexcept = default;

		Window& operator = (const Window& other) noexcept = default;
		Window& operator = (Window&& other) noexcept = default;

		~Window() noexcept;

		bool create(int width = 1024, int height = 1024);

		GLFWwindow* getInternal() noexcept { return internalWindow; }
		const GLFWwindow* getInternal() const noexcept { return internalWindow; }

		bool isOpen() const;

		bool shouldBeClosed() const;

		static void FramebufferSizeCallback(GLFWwindow* internalWindow, int width, int height);

		void setWindowEvents(WindowEvents* newEvent) { windowEvents = newEvent; }

		const WindowEvents& getWindowEvents() const noexcept { return *windowEvents; }
		WindowEvents& getWindowEvents() noexcept { return *windowEvents; }

		bool isMinimized() const;

		void requestCloseWindow();

		void setWindowResizedCallback(void* userPointer, WindowResizedCallback callback);

		WindowResizedCallback getWindowResizedCallback() const noexcept { return windowResizedCallback; }

		void* getWindowResizedCallbackUserPointer() noexcept { return windowResizedCallbackUserPointer; }

		void swapBuffers();

		static void SetTransparentFramebuffer(bool value);

		/// For Vulkan sometimes it's necessary to call this function too
		void makeWindowTransparentWhileUsingVulkan();

		Vector2i getFramebufferSize() const;

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

	inline Vector2i Window::getFramebufferSize() const
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
			windowResizedCallback = callback;
			windowResizedCallbackUserPointer = userPointer;
		}
		else
		{
			Logger->error("Can't bind user pointer: {} : Is callback valid: {}", userPointer, static_cast<bool>(callback));
		}
	}
}