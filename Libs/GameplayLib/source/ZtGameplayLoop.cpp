#include "Zinet/GameplayLib/ZtGameplayLoop.hpp"
#include "Zinet/GameplayLib/ZtFlipbook.hpp"

#include "Zinet/Window/ZtGLFW.hpp"
#include "Zinet/Window/ZtWindow.hpp"
#include "Zinet/Window/ZtWindowEvents.hpp"

#include <ranges>

namespace zt::gameplay_lib
{
	void GameplayLoop::start()
	{
		ActiveGameplayLoop = std::shared_ptr<GameplayLoop>(this, [](auto*) {});

		wd::GLFW::Init(false);

		window.setWindowEvents(windowEvents.get());

		wd::Window::SetTransparentFramebuffer(true);

		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		if (!monitor)
			return;

		const GLFWvidmode* vidMode = glfwGetVideoMode(monitor);
		if (!vidMode)
			return;

		//window.create(vidMode->width, vidMode->height);
		window.create();
		windowEvents->bindCallbacks();
		windowEventsSystem.setWindowEvents(windowEvents);

		if (!openGLRenderer.init(window))
		{
			Logger->error("OpenGL renderer failed init");
			return;
		}

		openGLRenderer.preRender();
		loopClock.start();
		fpsCounterClock.start();
		while (!window.shouldBeClosed())
		{
#if ZINET_TIME_TRACE
			core::Clock clock;
			clock.start();
#endif
			windowEvents->pollEvents();

			if (!drawableSystem.getCurrentCamera())
			{
				Logger->error("Camera is invalid");
				continue;
			}

			const auto deltaTimeMs = loopClock.restart().getAsMilliseconds();
			tickableSystem.tick(deltaTimeMs);
			drawableSystem.tick(deltaTimeMs);
			windowEventsSystem.tick(deltaTimeMs);

			auto currentCamera = drawableSystem.getCurrentCamera();
			auto viewportRenderTarget = currentCamera->getViewportRenderTarget();
			openGLRenderer.setupTexture(viewportRenderTarget.getResolution(), viewportRenderTarget.get());
			openGLRenderer.render();

			window.swapBuffers();

			++fpsCounter;
			if (fpsCounterClock.getElapsedTime().getAsSeconds() >= 1.f)
			{
				fpsCounterClock.restart();
				windowTitle = fmt::format("Zinet FPS: {}", fpsCounter);
				window.setTitle(windowTitle);
				fpsCounter = 0u;
			}

#if ZINET_TIME_TRACE
			const auto elapsedTime = clock.getElapsedTime().getAsMilliseconds();
			Logger->info("Frame took: {} milliseconds", elapsedTime);
#endif
		}
		openGLRenderer.postRender();

		window.destroyWindow();
		wd::GLFW::Deinit();
	}

}