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
	void Renderer::Init(ecs::World& world)
	{
		auto rendererRes = world.addResource(vulkan_renderer::VulkanRenderer{});
		if (!rendererRes)
		{
			Logger->error("Couldn't add a renderer res to the world");
			
			auto exitReasonRes = world.getResource<components::ExitReason>();
			if (exitReasonRes)
			{
				exitReasonRes->reason = "Couldn't add a renderer to the world";
				exitReasonRes->exit = true;
				return;
			}
		}

		auto windowRes = world.getResource<wd::Window>();
		if (!windowRes)
		{
			Logger->error("Couldn't get a window res from the world");
			return;
		}

		if (!rendererRes->init(*windowRes))
		{
			Logger->error("Renderer init failed");
			return;
		}
	}

	void Renderer::Update(ecs::World& world)
	{
		auto windowRes = world.getResource<wd::Window>();
		if (!windowRes)
		{
			Logger->error("Couldn't find a window resource");
			return;
		}

		if (windowRes->isMinimized())
		{
			// Skip rendering when the window is minimized
			// Because the swapchain can't be of size 0x0
			return;
		}

		auto rendererRes = world.getResource<vulkan_renderer::VulkanRenderer>();
		if (!rendererRes)
		{
			Logger->error("Renderer resource is invalid");
			return;
		}

		if (!rendererRes->nextImage())
		{
			Logger->error("Renderer couldn't switch to next image");
			return;
		}

		rendererRes->startRecordingDrawCommands();

		rendererRes->beginRenderPass(rendererRes->getRendererContext().getRenderPass());

		for (auto [renderDrawData] : ecs::Query<component::RenderDrawData>(world))
		{
			rendererRes->draw(renderDrawData->command);
		}

		rendererRes->endRenderPass();

		rendererRes->endRecordingDrawCommands();

		if (!rendererRes->submitCurrentDisplayImage())
		{
			Logger->error("Renderer couldn't submit draw commands");
			return;
		}

		if (!rendererRes->displayCurrentImage())
		{
			Logger->error("Renderer couldn't display current image");
			return;
		}
	}

	void Renderer::Deinit(ecs::World& world)
	{
		auto rendererRes = world.getResource<vulkan_renderer::VulkanRenderer>();
		if (!rendererRes)
		{
			Logger->info("Couldn't get a renderer from the world");
			return;
		}

		rendererRes->deinit();
	}
}