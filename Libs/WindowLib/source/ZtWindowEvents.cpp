#include "Zinet/Window/ZtWindowEvents.hpp"

namespace zt::wd
{
	void WindowEvents::bindCallbacks()
	{
		keyboard.bindCallbacks();
		mouse.bindCallbacks();
	}

	void WindowEvents::pollEvents()
	{
		keyboard.clearEvents();
		mouse.clearEvents();
		glfwPollEvents();
	}

	std::string WindowEvents::asString() const
	{
		return fmt::format("Window events:\n{}{}", keyboard.asString(), mouse.asString());
	}

}