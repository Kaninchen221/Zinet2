#include "Zinet/Window/ZtEvent.hpp"

namespace zt::wd
{
	void Event::bindCallbacks()
	{
		keyboard.bindCallbacks();
		mouse.bindCallbacks();
	}

	void Event::pollEvents()
	{
		keyboard.clearEvents();
		mouse.clearEvents();
		glfwPollEvents();
	}

	std::string Event::asString() const
	{
		return fmt::format("Window events:\n{}{}", keyboard.asString(), mouse.asString());
	}

}