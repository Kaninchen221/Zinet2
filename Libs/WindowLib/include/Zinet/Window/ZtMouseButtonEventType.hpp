#pragma once

#include "Zinet/Window/ZtWindowConfig.hpp"

namespace zt::wd
{
	enum class MouseButtonEventType
	{
		Pressed = GLFW_PRESS,
		Repeated = GLFW_REPEAT,
		Released = GLFW_RELEASE
	};

}