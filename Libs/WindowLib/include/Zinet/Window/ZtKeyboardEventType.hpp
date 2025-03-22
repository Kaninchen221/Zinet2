#pragma once

#include "Zinet/Window/ZtWindowConfig.hpp"

namespace zt::wd
{

	enum class KeyboardEventType
	{
		Ignore = 0,
		Pressed = GLFW_PRESS,
		Repeated = GLFW_REPEAT,
		Released = GLFW_RELEASE
	};

}