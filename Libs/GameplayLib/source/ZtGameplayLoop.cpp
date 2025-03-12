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

		window.create();

		if (!openGLRenderer.init(window))
		{
			Logger->error("OpenGL renderer failed init");
			return;
		}

		finalRenderTarget.createEmpty({ 1920, 1080 }, sf::ColorFormat::R8G8B8A8_SRGB);

		openGLRenderer.preRender();
		while (!window.shouldBeClosed())
		{
#if ZINET_TIME_TRACE
			core::Clock clock;
			clock.start();
#endif
			event.pollEvents();

			finalRenderTarget.fill(sf::BlackColor);

			for (const auto& object : tickableObjects)
			{
				if (object.expired())
					continue;

				auto asShared = object.lock();
				asShared->tick(20.f);
			}

			for (const auto& object : drawableObjects)
			{
				if (object.expired())
					continue;

				auto asShared = object.lock();
				auto drawInfo = asShared->getDrawInfo();
				softwareRenderer.draw(drawInfo, finalRenderTarget);
			}

			openGLRenderer.setupTexture(finalRenderTarget.getResolution(), finalRenderTarget.get());
			openGLRenderer.render();

			window.swapBuffers();
#if ZINET_TIME_TRACE
			const auto elapsedTime = clock.getElapsedTime().getAsMilliseconds();
			Logger->info("Frame took: {} milliseconds", elapsedTime);
#endif
		}
		openGLRenderer.postRender();
	}

	void GameplayLoop::addTickable(const std::weak_ptr<Tickable>& tickable)
	{
		tickableObjects.push_back(tickable);
	}

	void GameplayLoop::addDrawable(const std::weak_ptr<Drawable>& drawable)
	{
		drawableObjects.push_back(drawable);
	}

}