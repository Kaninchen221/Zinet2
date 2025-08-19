#include "Zinet/Window/ZtWindow.hpp"

#include "Zinet/Core/ZtDebug.hpp"

#include <type_traits>
#include <source_location>

#if ZINET_WINDOWS
#	define GLFW_EXPOSE_NATIVE_WIN32
#	include <GLFW/glfw3native.h>
#endif

namespace zt::wd
{

    Window::~Window() noexcept
	{
		if (internalWindow)
			destroyWindow();
    }

    bool Window::create(int width, int height)
	{
		Logger->info(std::source_location::current().function_name());

		internalWindow = glfwCreateWindow(width, height, "Zinet", NULL, NULL);
        if (internalWindow == nullptr)
        {
            Logger->error("Can't create window");
            return false;
        }

		// OpenGL only
        //glfwMakeContextCurrent(internalWindow);

        glfwSetWindowUserPointer(internalWindow, this);

        // Window must be always at least bigger as 1 x 1 because of the swap chain ~ Vulcan
        glfwSetWindowSizeLimits(internalWindow, 1, 1, GLFW_DONT_CARE, GLFW_DONT_CARE);

		bindFramebufferSizeCallback();

		return true;
    }

    bool Window::isOpen() const
    {
        if (!internalWindow)
            return false;

        return !glfwWindowShouldClose(internalWindow);
    }

    bool Window::shouldBeClosed() const
	{
		if (!internalWindow)
			return false;

        return glfwWindowShouldClose(internalWindow);
    }

    void Window::FramebufferSizeCallback(GLFWwindow* internalWindow, int width, int height)
    {
        void* windowUserPointer = glfwGetWindowUserPointer(internalWindow);
        if (windowUserPointer == nullptr)
        {
            Logger->warn("Window::FramebufferSizeCallback: windowUserPointer is nullptr");
            return;
        }

        Window* window = static_cast<Window*>(windowUserPointer);
		if (window->windowResizedCallback && window->windowResizedCallbackUserPointer)
		{
            auto callable = std::bind(window->windowResizedCallback, window->windowResizedCallbackUserPointer, Vector2i{width, height});
			std::invoke(callable);
		}
    }

	bool Window::isMinimized() const
	{
        Vector2i windowSize = getSize();
        return windowSize.x == 0 && windowSize.y == 0;
	}

	void Window::requestCloseWindow()
	{
		if (!internalWindow)
			return;

        glfwSetWindowShouldClose(internalWindow, true);
	}

	void Window::swapBuffers()
	{
		if (!internalWindow)
			return;

        glfwSwapBuffers(internalWindow);
	}

	void Window::SetTransparentFramebuffer(bool value)
	{
		glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, value ? GLFW_TRUE : GLFW_FALSE);
	}

	void Window::setShowWindowBar(bool value)
	{
		if (!internalWindow)
			return;

		glfwSetWindowAttrib(internalWindow, GLFW_DECORATED, value ? GLFW_TRUE : GLFW_FALSE);
	}

	void Window::bindFramebufferSizeCallback()
	{
		if (!internalWindow)
			return;

		glfwSetFramebufferSizeCallback(internalWindow, &Window::FramebufferSizeCallback);
	}

	void Window::setTitle(const std::string& title)
	{
		/// https://www.glfw.org/docs/latest/window_guide.html#window_title
		/// It's copied so we don't need to hold a copy
		glfwSetWindowTitle(internalWindow, title.c_str());
	}

	void Window::makeWindowTransparentWhileUsingVulkan()
	{
		// TODO: Linux version
		// Enable transparent framebuffer on windows platform
#		if ZINET_WINDOWS
		auto hwnd = glfwGetWin32Window(getInternal());

		SetWindowLong(hwnd, GWL_EXSTYLE,
		GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED);
		SetLayeredWindowAttributes(hwnd, 0, static_cast<BYTE>(255), LWA_ALPHA);
#		else
		TerminateDebug("makeWindowTransparentWhileUsingVulkan is not implemented for this platform");
#		endif // ZINET_WINDOWS
	}

	void Window::destroyWindow()
	{
		if (internalWindow)
		{
			glfwDestroyWindow(internalWindow);
			internalWindow = nullptr;
		}
	}

}