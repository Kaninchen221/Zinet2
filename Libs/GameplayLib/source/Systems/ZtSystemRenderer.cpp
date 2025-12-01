#include "Zinet/Gameplay/Systems/ZtSystemRenderer.hpp"

#include "Zinet/Gameplay/ZtRenderDrawData.hpp"

#include "Zinet/Core/ECS/ZtWorld.hpp"
#include "Zinet/Core/ECS/ZtQuery.hpp"
#include "Zinet/Core/ZtExitReason.hpp"

#include "Zinet/Window/ZtWindow.hpp"

#include "Zinet/VulkanRenderer/ZtVulkanRenderer.hpp"

using namespace zt::core;

namespace zt::gameplay::system
{
	void Renderer::Init(ecs::WorldCommands worldCommands, ecs::Resource<wd::Window> windowRes)
	{
		if (!windowRes)
		{
			worldCommands.addResource(ExitReason{ "Couldn't find a window resource" });
			return;
		}

		vulkan_renderer::VulkanRenderer vulkanRenderer;
		if (!vulkanRenderer.init(*windowRes))
		{
			worldCommands.addResource(ExitReason{ "Couldn't init renderer" });
			return;
		}

		worldCommands.addResource(vulkanRenderer);
	}

	void Renderer::Update(
		ecs::WorldCommands worldCommands,
		ecs::ConstResource<wd::Window> windowRes,
		ecs::Resource<vulkan_renderer::VulkanRenderer> rendererRes,
		ecs::ConstQuery<RenderDrawData> drawDataQuery)
	{
		if (!windowRes)
			return;

		if (windowRes->isMinimized())
		{
			Logger->trace("Window is minimized so skip rendering");
			return;
		}

		if (!rendererRes)
		{
			worldCommands.addResource(ExitReason{ "Expected renderer res" });
			return;
		}

		if (!rendererRes->nextImage())
		{
			worldCommands.addResource(ExitReason{ "Renderer couldn't switch to next image" });
			return;
		}

		rendererRes->startRecordingDrawCommands();

		rendererRes->beginRenderPass(rendererRes->getRendererContext().getRenderPass());

		for (auto [drawData] : drawDataQuery)
		{
			if (drawData->shouldDraw)
				rendererRes->draw(drawData->command);
		}

		rendererRes->endRenderPass();

		rendererRes->endRecordingDrawCommands();

		if (!rendererRes->submitCurrentDisplayImage())
		{
			worldCommands.addResource(ExitReason{ "Renderer couldn't submit draw commands" });
			return;
		}

		if (!rendererRes->displayCurrentImage())
		{
			worldCommands.addResource(ExitReason{ "Renderer couldn't display current image" });
			return;
		}
	}

	void Renderer::Deinit(ecs::Resource<vulkan_renderer::VulkanRenderer> rendererRes)
	{
		if (!rendererRes)
			return;

		rendererRes->deinit();
	}
}