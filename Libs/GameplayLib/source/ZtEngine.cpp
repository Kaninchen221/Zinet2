#include "Zinet/Gameplay/ZtEngine.hpp"

#include "imgui.h"

namespace zt::gameplay
{
	bool Engine::init() noexcept
	{
		if (!engineContext.init())
			return false;

		return true;
	}

	void Engine::loop() noexcept
	{
		engineContext.loop();
	}

	void Engine::deinit() noexcept
	{
		engineContext.deinit();
	}

}