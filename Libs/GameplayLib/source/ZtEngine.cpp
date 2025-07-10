#include "Zinet/Gameplay/ZtEngine.hpp"

#include "imgui.h"

namespace zt::gameplay
{
	bool Engine::init() noexcept
	{
		systemRenderer.init();

		//imGuiIntegration.init(renderer.getRendererContext(), window);

		return true;
	}

	void Engine::loop() noexcept
	{
		windowEvents.pollEvents();

		// Game logic

		// Rendering
		systemRenderer.update();

	}

	void Engine::deinit() noexcept
	{
		systemRenderer.deinit();
	}

}