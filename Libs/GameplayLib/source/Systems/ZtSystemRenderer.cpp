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
	void Renderer::Init(ecs::World&)
	{
// 		auto rendererRes = world.addResource(vulkan_renderer::VulkanRenderer{});
// 		if (!rendererRes)
// 		{
// //			return { Level::Error, "Couldn't add a renderer res to the world" };
// 		}
// 
// 		auto windowRes = world.getResource<wd::Window>();
// 		if (!windowRes)
// 		{
// //			return { Level::Error, "Couldn't get a window res from the world" };
// 		}
// 
// //		if (!rendererRes->init(*windowRes))
// 		{
// //			return { Level::Error, "Renderer init failed" };
// //		}
	}

	void Renderer::Update(ecs::World&)
	{
// 		auto windowRes = world.getResource<wd::Window>();
// 		if (!windowRes)
// 		{
// //			return { Level::Error, "Couldn't find a window resource" };
// 		}
// 
// 		if (windowRes->isMinimized())
// 		{
// //			return { Level::Info, "Skip rendering because window is minimized" };
// 		}
// 
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
	}

	void Renderer::Deinit(ecs::World&)
	{
// 		auto rendererRes = world.getResource<vulkan_renderer::VulkanRenderer>();
// 		if (!rendererRes)
// 		{
// //			return { Level::Warn, "Couldn't get a renderer from the world" };
// 		}
// 
// 		rendererRes->deinit();
	}
}