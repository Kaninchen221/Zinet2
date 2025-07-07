#include "Zinet/Gameplay/ZtGameplayLoop.hpp"

#include "imgui.h"

namespace zt::gameplay
{
	bool GameplayLoop::init() noexcept
	{
		wd::GLFW::Init(false);

		zt::wd::Window::SetTransparentFramebuffer(true);
		window.create(800, 800);
		window.makeWindowTransparentWhileUsingVulkan();

		renderer.init(window);

		imGuiIntegration.init(renderer.getRendererContext(), window);

		return true;
	}

	void GameplayLoop::loop() noexcept
	{
		windowEvents.pollEvents();

		// Game logic

		//imGuiIntegration.implSpecificNewFrame();

		//ImGui::NewFrame();

		//ImGui::EndFrame();

		//updateUniformBuffersData();

		//renderer.createPipeline(drawInfo);
		//renderer.getGraphicsPipeline().isValid();

		//renderer.beginFrame();

		//imGuiIntegration.prepareRenderData();

		//renderer.draw(drawInfo);

		//renderer.submit();

		//renderer.endFrame();

	}

	void GameplayLoop::shutdown() noexcept
	{
		const auto& device = renderer.getRendererContext().device;
		device.waitIdle();

		imGuiIntegration.deinit(renderer.getRendererContext());
		renderer.shutdown();
		window.destroyWindow();
		wd::GLFW::Deinit();
	}

}