#include "Zinet/Gameplay/Systems/ZtSystemImGui.hpp"

#include "Zinet/Core/ECS/ZtWorld.hpp"

#include "Zinet/VulkanRenderer/ZtImGuiIntegration.hpp"
#include "Zinet/VulkanRenderer/ZtVulkanRenderer.hpp"

#include "Zinet/Window/ZtWindow.hpp"

namespace zt::gameplay::system
{
	void ImGui::Init(core::ecs::World& world)
	{
		auto windowRes = world.getResource<wd::Window>();
		if (!windowRes)
		{
			Logger->error("Couldn't get a window res in the world");
			return;
		}

		auto rendererRes = world.getResource<vulkan_renderer::VulkanRenderer>();
		if (!rendererRes)
		{
			Logger->error("Couldn't get a renderer res from the world");
			return;
		}

		auto imGuiIntegrationRes = world.addResource(vulkan_renderer::ImGuiIntegration{});
		if (!imGuiIntegrationRes)
		{
			Logger->error("Couldn't add an imGuiIntegration to the world");
			return;
		}

		if (!imGuiIntegrationRes->init(rendererRes->getRendererContext(), *windowRes))
		{
			Logger->error("Couldn't init imgui integration");
			return;
		}
	}

	void ImGui::Deinit(core::ecs::World& world)
	{
		auto rendererRes = world.getResource<vulkan_renderer::VulkanRenderer>();
		if (!rendererRes)
		{
			Logger->error("Couldn't get a renderer res from the world");
			return;
		}

		auto imGuiIntegrationRes = world.getResource<vulkan_renderer::ImGuiIntegration>();
		if (!imGuiIntegrationRes)
		{
			Logger->error("Couldn't get an imGuiIntegration to the world");
			return;
		}

		imGuiIntegrationRes->deinit(rendererRes->getRendererContext());
	}
}