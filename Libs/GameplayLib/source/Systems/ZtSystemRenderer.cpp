#include "Zinet/Gameplay/Systems/ZtSystemRenderer.hpp"

#include "Zinet/Gameplay/ZtRenderCommand.hpp"

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
		ecs::ConstQuery<RenderCommand> drawCommandQuery,
		DrawDataQuery drawDataQuery,
		ecs::Resource<vulkan_renderer::ResourceStorage> resourceStorageRes)
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

		for (auto [graphicsPipeline, drawInfo] : drawDataQuery)
		{
			if (graphicsPipeline->isValid())
				rendererRes->draw(*graphicsPipeline, *drawInfo);
		}

		for (auto [drawCommand] : drawCommandQuery)
		{
			if (drawCommand->shouldDraw)
				rendererRes->draw(drawCommand->command);
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

		// Create requested resources
		// TODO: Maybe move it to another system?
		resourceStorageRes->createResources(rendererRes->getRendererContext());
	}

	void Renderer::Deinit(
		ecs::Resource<vulkan_renderer::VulkanRenderer> rendererRes,
		ecs::Resource<vulkan_renderer::ResourceStorage> resourceStorageRes)
	{
		if (!rendererRes)
			return;

		resourceStorageRes->clear(rendererRes->getRendererContext());

		rendererRes->deinit();
	}
}