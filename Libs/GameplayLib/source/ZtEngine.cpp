#include "Zinet/Gameplay/ZtEngine.hpp"

#include "imgui.h"

namespace zt::gameplay
{
	bool Engine::init() ZINET_API_POST
	{
		if (!engineContext.init())
			return false;

		return true;
	}

	void Engine::loop() ZINET_API_POST
	{
		engineContext.loop();
	}

	void Engine::deinit() ZINET_API_POST
	{
		engineContext.deinit();
	}

}