#include "Zinet/GameplayLib/ZtGameplayLoop.hpp"

#include "Zinet/Window/ZtGLFW.hpp"
#include "Zinet/Window/ZtWindow.hpp"

#include "Zinet/GameplayLib/ZtFlipbook.hpp"

namespace zt::gameplay_lib
{
	void GameplayLoop::start()
	{
		wd::GLFW glfw;
		wd::GLFW::UnhideWindow();

		wd::Window window;
		wd::Event event{ window };

		wd::Window::SetTransparentFramebuffer(true);

		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		if (!monitor)
			return;

		const GLFWvidmode* vidMode = glfwGetVideoMode(monitor);
		if (!vidMode)
			return;

		//window.create(vidMode->width, vidMode->height);
		window.create();

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
			event.pollEvents();

			if (!currentCamera)
			{
				Logger->error("Camera is invalid");
				continue;
			}

			auto& viewportRenderTarget = currentCamera->getViewportRenderTarget();
			viewportRenderTarget.fill(sf::ZeroColor);

			const auto deltaTimeMs = loopClock.restart().getAsMilliseconds();
			for (const auto& object : tickableObjects)
			{
				if (object.expired())
					continue;

				auto asShared = object.lock();
				asShared->tick(deltaTimeMs);
			}

			for (const auto& object : drawableObjects)
			{
				if (object.expired())
					continue;

				auto asShared = object.lock();
				auto drawInfo = asShared->getDrawInfo();
				auto& vertexShader = drawInfo.shaderProgram.vertexShader;
				vertexShader.cameraPosition = currentCamera->getPosition();
				vertexShader.cameraSize = currentCamera->getViewportRenderTarget().getResolution();
				softwareRenderer.draw(drawInfo, viewportRenderTarget);
			}

			openGLRenderer.setupTexture(viewportRenderTarget.getResolution(), viewportRenderTarget.get());
			openGLRenderer.render();

			window.swapBuffers();
#if ZINET_TIME_TRACE
			const auto elapsedTime = clock.getElapsedTime().getAsMilliseconds();
			Logger->info("Frame took: {} milliseconds", elapsedTime);
#endif
		}
		openGLRenderer.postRender();
	}

	void GameplayLoop::addTickable(const std::weak_ptr<Node>& tickable)
	{
		tickableObjects.push_back(tickable);
	}

	void GameplayLoop::addDrawable(const std::weak_ptr<Node>& drawable)
	{
		drawableObjects.push_back(drawable);
	}

}