#pragma once

#include "Zinet/Window/ZtWindowConfig.hpp"
#include "Zinet/Window/ZtKeyboardEventType.hpp"
#include "Zinet/Window/ZtKeyboardKey.hpp"

#include <cstdint>

namespace zt::wd
{
	enum class KeyboardMods
	{
		Ignore = 0,
		Shift = GLFW_MOD_SHIFT,
		Control = GLFW_MOD_CONTROL,
		Alt = GLFW_MOD_ALT,
		Super = GLFW_MOD_SUPER,
		CapsLock = GLFW_MOD_CAPS_LOCK,
		NumLock = GLFW_MOD_NUM_LOCK
	};

	struct ZINET_WINDOW_LAYER_API KeyboardEvent
	{
		KeyboardEventType type = KeyboardEventType::Ignore;
		KeyboardKey key = KeyboardKey::Ignore;
		KeyboardMods mods = KeyboardMods::Ignore;

		bool operator == (const KeyboardEvent& other) const;
	};

	inline bool KeyboardEvent::operator==(const KeyboardEvent& other) const
	{
		if (type == other.type && key == other.key && mods == other.mods)
			return true;
		else
			return false;
	}

}
