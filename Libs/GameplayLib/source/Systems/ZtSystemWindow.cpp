#include "Zinet/Gameplay/Systems/ZtSystemWindow.hpp"
#include "Zinet/Gameplay/ZtEngineContext.hpp"

namespace zt::gameplay
{

	bool SystemWindow::init()
	{
		System::init();

		wd::GLFW::Init(false);

		if (!window.create())
			return false;

		return true;
	}

	void SystemWindow::update()
	{
		System::update();

		windowEvents.pollEvents();

		// Handle window close
		// One of the situations is when user clicks the "X" button on the window
		if (window.shouldBeClosed())
		{
			window.requestCloseWindow();

			auto& engineContext = EngineContext::Get();
			engineContext.stopLooping();
		}
	}

	bool SystemWindow::deinit()
	{
		window.requestCloseWindow();

		// Wait until the window is closed
		while (window.isOpen()) {}

		window.destroyWindow();

		wd::GLFW::Deinit();

		initialized = false;
		return true;
	}

}