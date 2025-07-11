#include "Zinet/Gameplay/ZtEngineContext.hpp"

#include "imgui.h"

namespace zt::gameplay
{
	bool EngineContext::init() noexcept
	{
		wd::GLFW::Init(false);

		if (!window.create())
			return false;

		if (!systemRenderer.init(*this))
			return false;

		if (!systemImGui.init(*this))
			return false;

		return true;
	}

	void EngineContext::loop() noexcept
	{
		windowEvents.pollEvents();

		systemImGui.update();
		systemRenderer.update();
	}

	void EngineContext::deinit() noexcept
	{
		systemImGui.deinit();
		systemRenderer.deinit();

		window.destroyWindow();
		wd::GLFW::Deinit();
	}

}