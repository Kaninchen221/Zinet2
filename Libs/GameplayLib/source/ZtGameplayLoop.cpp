#include "Zinet/Gameplay/ZtGameplayLoop.hpp"

#include "imgui.h"

namespace zt::gameplay
{
	bool GameplayLoop::init() noexcept
	{
		systemRenderer.init();

		//imGuiIntegration.init(renderer.getRendererContext(), window);

		return true;
	}

	void GameplayLoop::loop() noexcept
	{
		windowEvents.pollEvents();

		// Game logic

		// Rendering
		systemRenderer.update();

	}

	void GameplayLoop::deinit() noexcept
	{
		systemRenderer.deinit();
	}

}