#include "Zinet/Window/ZtWindow.hpp"

#include "Zinet/Core/ZtDebug.hpp"

#include <type_traits>
#include <source_location>

namespace zt::wd
{

    Window::~Window() noexcept
    {
		Logger->info(std::source_location::current().function_name());

        if (internalWindow != nullptr)
        {
            glfwDestroyWindow(internalWindow);
        }
    }

    void Window::create()
	{
		Logger->info(std::source_location::current().function_name());

        internalWindow = glfwCreateWindow(1024, 1024, "Zinet", NULL, NULL);
        if (internalWindow == nullptr)
        {
            Logger->error("Can't create window");
            return;
        }

        glfwMakeContextCurrent(internalWindow);

        glfwSetWindowUserPointer(internalWindow, this);

        // Window must be always at least bigger as 1 x 1 because of the swap chain ~ Vulcan
        glfwSetWindowSizeLimits(internalWindow, 1, 1, GLFW_DONT_CARE, GLFW_DONT_CARE);

		bindFramebufferSizeCallback();
    }

    GLFWwindow* Window::getInternal()
    {
        return internalWindow;
    }

    const GLFWwindow* Window::getInternal() const
    {
        return internalWindow;
    }

    void Window::bindCallbacks()
    {
        event.bindCallbacks();
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
		if (window->windowResizedCallbackPointer && window->windowResizedCallbackUserPointer)
		{
            auto callable = std::bind(window->windowResizedCallbackPointer, window->windowResizedCallbackUserPointer, Vector2ui{width, height});
			std::invoke(callable);
		}
    }

    Vector2ui Window::getSize() const
	{
        int width = 0;
        int height = 0;
		glfwGetFramebufferSize(internalWindow, &width, &height);
        return { width, height };
	}

	bool Window::isMinimized() const
	{
        Vector2i windowSize = getSize();
        return windowSize.x == 0 && windowSize.y == 0;
	}

	void Window::requestCloseWindow()
	{
        glfwSetWindowShouldClose(internalWindow, true);
	}

	void Window::swapBuffers()
	{
        glfwSwapBuffers(internalWindow);
	}

	void Window::bindFramebufferSizeCallback()
	{
		if (!internalWindow)
			return;

		glfwSetFramebufferSizeCallback(internalWindow, &Window::FramebufferSizeCallback);
	}
}