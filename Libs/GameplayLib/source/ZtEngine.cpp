#include "Zinet/Gameplay/ZtEngine.hpp"

#include "imgui.h"

namespace zt::gameplay
{
	bool Engine::init()
	{
		if (!engineContext.init())
			return false;

		return true;
	}

	void Engine::loop()
	{
		engineContext.loop();
	}

	void Engine::deinit()
	{
		engineContext.deinit();
	}

}