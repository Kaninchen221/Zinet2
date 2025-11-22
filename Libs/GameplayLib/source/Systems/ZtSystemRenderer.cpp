#include "Zinet/Gameplay/Systems/ZtSystemRenderer.hpp"

#include "Zinet/Gameplay/Components/ZtRenderDrawData.hpp"

#include "Zinet/Core/ECS/ZtWorld.hpp"
#include "Zinet/Core/ECS/ZtQuery.hpp"
#include "Zinet/Core/Components/ZtExitReason.hpp"

#include "Zinet/Window/ZtWindow.hpp"

#include "Zinet/VulkanRenderer/ZtVulkanRenderer.hpp"

using namespace zt::core;

namespace zt::gameplay::system
{
	void Renderer::Init(ecs::WorldCommands worldCommands, ecs::Resource<wd::Window> windowRes)
	{
		if (!windowRes)
		{
			worldCommands.addResource(ExitReason{ true, "Couldn't find a window resource" });
			return;
		}

		vulkan_renderer::VulkanRenderer vulkanRenderer;
		if (!vulkanRenderer.init(*windowRes))
		{
			worldCommands.addResource(ExitReason{ true, "Couldn't init renderer" });
			return;
		}

		worldCommands.addResource(vulkanRenderer);
	}

	void Renderer::Update(
		ecs::WorldCommands worldCommands,
		ecs::Resource<wd::Window> windowRes,
		ecs::Resource<vulkan_renderer::VulkanRenderer> rendererRes,
		ecs::ConstQuery<component::RenderDrawData> drawDataQuery)
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
			worldCommands.addResource(ExitReason{ true, "Expected renderer res" });
			return;
		}

		if (!rendererRes->nextImage())
		{
			worldCommands.addResource(ExitReason{ true, "Renderer couldn't switch to next image" });
			return;
		}

		rendererRes->startRecordingDrawCommands();

		rendererRes->beginRenderPass(rendererRes->getRendererContext().getRenderPass());

		for (auto [drawData] : drawDataQuery)
		{
			rendererRes->draw(drawData->command);
		}

		rendererRes->endRenderPass();

		rendererRes->endRecordingDrawCommands();

		if (!rendererRes->submitCurrentDisplayImage())
		{
			worldCommands.addResource(ExitReason{ true, "Renderer couldn't submit draw commands" });
			return;
		}

		if (!rendererRes->displayCurrentImage())
		{
			worldCommands.addResource(ExitReason{ true, "Renderer couldn't display current image" });
			return;
		}
	}

// 	void Renderer::Update(ecs::World&)
// 	{
// 		auto rendererRes = world.getResource<vulkan_renderer::VulkanRenderer>();
// 		if (!rendererRes)
// 		{
// //			return { Level::Error, "Renderer resource is invalid" };
// 		}
// 
// 		if (!rendererRes->nextImage())
// 		{
// //			return { Level::Error, "Renderer couldn't switch to next image" };
// 		}
// 
// 		rendererRes->startRecordingDrawCommands();
// 
// 		rendererRes->beginRenderPass(rendererRes->getRendererContext().getRenderPass());
// 
// 		for (auto [renderDrawData] : ecs::Query<component::RenderDrawData>(world))
// 		{
// 			rendererRes->draw(renderDrawData->command);
// 		}
// 
// 		rendererRes->endRenderPass();
// 
// 		rendererRes->endRecordingDrawCommands();
// 
// 		if (!rendererRes->submitCurrentDisplayImage())
// 		{
// //			return { Level::Error, "Renderer couldn't submit draw commands" };
// 		}
// 
// 		if (!rendererRes->displayCurrentImage())
// 		{
// //			return { Level::Error, "Renderer couldn't display current image" };
// 		}
/*	}*/

	void Renderer::Deinit(ecs::Resource<vulkan_renderer::VulkanRenderer> rendererRes)
	{
		if (!rendererRes)
			return;

		rendererRes->deinit();
	}
}