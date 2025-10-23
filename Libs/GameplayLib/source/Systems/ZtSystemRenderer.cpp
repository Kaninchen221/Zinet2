#include "Zinet/Gameplay/Systems/ZtSystemRenderer.hpp"

#include "Zinet/Core/ECS/ZtWorld.hpp"
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

	void Renderer::Update([[maybe_unused]] ecs::World& world)
	{

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