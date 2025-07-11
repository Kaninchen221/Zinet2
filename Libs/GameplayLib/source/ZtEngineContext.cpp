#include "Zinet/Gameplay/ZtEngineContext.hpp"

#include "imgui.h"

namespace zt::gameplay
{
	bool EngineContext::init() noexcept
	{
		instance = this;
		
		wd::GLFW::Init(false);

		if (!window.create())
			return false;

		if (!systemRenderer.init())
			return false;

		if (!systemImGui.init())
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

		instance = nullptr;
	}

	EngineContext::~EngineContext() noexcept
	{
		if (instance)
		{
			Logger->error("EngineContext must be manually deinitialized");
		}
	}

}