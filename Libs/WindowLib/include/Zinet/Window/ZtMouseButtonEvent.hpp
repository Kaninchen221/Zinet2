#pragma once

#include "Zinet/Window/ZtWindowConfig.hpp"
#include "Zinet/Window/ZtMouseButton.hpp"
#include "Zinet/Window/ZtMouseButtonEventType.hpp"

namespace zt::wd
{
	struct ZINET_WINDOW_LAYER_API MouseButtonEvent
	{
		MouseButtonEventType type;
		MouseButton button;

		bool operator == (const MouseButtonEvent& other) const
		{
			return type == other.type && button == other.button;
		}
	};

}