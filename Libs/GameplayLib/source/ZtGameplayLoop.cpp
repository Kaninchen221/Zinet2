#include "Zinet/GameplayLib/ZtGameplayLoop.hpp"
#include "Zinet/GameplayLib/ZtFlipbook.hpp"

#include "Zinet/Window/ZtGLFW.hpp"
#include "Zinet/Window/ZtWindow.hpp"
#include "Zinet/Window/ZtEvent.hpp"

#include <ranges>

namespace zt::gameplay_lib
{
	void GameplayLoop::start()
	{
		wd::GLFW::Init(false);

		window.setEvent(event.get());

		wd::Window::SetTransparentFramebuffer(true);

		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		if (!monitor)
			return;

		const GLFWvidmode* vidMode = glfwGetVideoMode(monitor);
		if (!vidMode)
			return;

		//window.create(vidMode->width, vidMode->height);
		window.create();
		event->bindCallbacks();
		dragableSystem.setEvent(event);

		if (!openGLRenderer.init(window))
		{
			Logger->error("OpenGL renderer failed init");
			return;
		}

		openGLRenderer.preRender();
		loopClock.start();
		while (!window.shouldBeClosed())
		{
#if ZINET_TIME_TRACE
			core::Clock clock;
			clock.start();
#endif
			event->pollEvents();

			if (!drawableSystem.getCurrentCamera())
			{
				Logger->error("Camera is invalid");
				continue;
			}

			const auto deltaTimeMs = loopClock.restart().getAsMilliseconds();
			tickableSystem.tick(deltaTimeMs);
			drawableSystem.tick(deltaTimeMs);
			dragableSystem.tick(deltaTimeMs);

			auto currentCamera = drawableSystem.getCurrentCamera();
			auto viewportRenderTarget = currentCamera->getViewportRenderTarget();
			openGLRenderer.setupTexture(viewportRenderTarget.getResolution(), viewportRenderTarget.get());
			openGLRenderer.render();

			window.swapBuffers();
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