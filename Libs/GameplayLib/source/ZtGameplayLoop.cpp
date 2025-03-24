#include "Zinet/GameplayLib/ZtGameplayLoop.hpp"
#include "Zinet/GameplayLib/ZtFlipbook.hpp"

#include "Zinet/Window/ZtGLFW.hpp"
#include "Zinet/Window/ZtWindow.hpp"
#include "Zinet/Window/ZtEvent.hpp"

namespace zt::gameplay_lib
{
	void GameplayLoop::start()
	{
		wd::GLFW glfw;
		wd::GLFW::UnhideWindow();

		wd::Window window;
		wd::Event event{ window };
		window.setEvent(&event);

		wd::Window::SetTransparentFramebuffer(true);

		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		if (!monitor)
			return;

		const GLFWvidmode* vidMode = glfwGetVideoMode(monitor);
		if (!vidMode)
			return;

		//window.create(vidMode->width, vidMode->height);
		window.create();
		event.bindCallbacks();

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
			for (const auto& node : tickableNodes)
			{
				if (node.expired())
					continue;

				auto asShared = node.lock();
				asShared->tick(deltaTimeMs);
			}

			for (const auto& node : drawableNodes)
			{
				if (node.expired())
					continue;

				auto asShared = node.lock();
				auto drawInfo = asShared->getDrawInfo();
				auto& vertexShader = drawInfo.shaderProgram.vertexShader;
				vertexShader.cameraPosition = currentCamera->getPosition();
				vertexShader.cameraSize = currentCamera->getViewportRenderTarget().getResolution();
				softwareRenderer.draw(drawInfo, viewportRenderTarget);
			}

			for (auto& node : dragableNodes)
			{
				if (node.expired())
					continue;

				auto& mouse = event.getMouse();

				const bool isPressed = mouse.isPressed(wd::MouseButton::LEFT);
				if (isPressed)
				{
					const auto mousePositionNorm = mouse.getMousePositionNorm();
					Logger->info("Mouse pos norm: {}, {}", mousePositionNorm.x, mousePositionNorm.y);

					if (node.expired())
						continue;

					const auto viewportSize = currentCamera->getViewportRenderTarget().getResolution();
					const auto lookAt = currentCamera->getLookAt();

					const Vector2f mousePosInWorld = { (mousePositionNorm.x * viewportSize.x) - (viewportSize.x / 2.f) + lookAt.x, (mousePositionNorm.y * viewportSize.y ) - (viewportSize.y / 2.f) + lookAt.y };

					auto asShared = node.lock();

					bool isHoveredByMouse = false;
					{
						auto nodePos = asShared->getPosition();
						auto nodeMin = nodePos;
						auto nodeMax = nodePos + asShared->getSize();
						if (mousePosInWorld.x > nodeMin.x && mousePosInWorld.x < nodeMax.x && mousePosInWorld.y > nodeMin.y && mousePosInWorld.y < nodeMax.y)
							isHoveredByMouse = true;
					}

					if (isHoveredByMouse)
					{
						const Vector2f newPosition = mousePosInWorld - asShared->getSize() / 2.f;
						asShared->setPosition(newPosition);
					}
				}
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

	void GameplayLoop::addTickable(const std::weak_ptr<Node>& node)
	{
		tickableNodes.push_back(node);
	}

	void GameplayLoop::addDrawable(const std::weak_ptr<Node>& node)
	{
		drawableNodes.push_back(node);
	}

	void GameplayLoop::addDragable(const std::weak_ptr<Node>& node)
	{
		dragableNodes.push_back(node);
	}

}