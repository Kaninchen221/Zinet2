#include "Zinet/Gameplay/Systems/ZtSystemImGui.hpp"

#include "Zinet/Gameplay/Components/ZtRenderDrawData.hpp"

#include "Zinet/Core/ECS/ZtWorld.hpp"

#include "Zinet/VulkanRenderer/ZtImGuiIntegration.hpp"
#include "Zinet/VulkanRenderer/ZtVulkanRenderer.hpp"

#include "Zinet/Window/ZtWindow.hpp"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>

using namespace zt::vulkan_renderer;

namespace zt::gameplay::system
{
	void ImGui::Init(core::ecs::World& world)
	{
		auto windowRes = world.getResource<wd::Window>();
		if (!windowRes)
		{
//			return { Level::Error, "Couldn't get a window res in the world" };
		}

		auto rendererRes = world.getResource<VulkanRenderer>();
		if (!rendererRes)
		{
//			return { Level::Error, "Couldn't get a renderer res from the world" };
		}

		auto imGuiIntegrationRes = world.addResource(ImGuiIntegration{});
		if (!imGuiIntegrationRes)
		{
//			return { Level::Error, "Couldn't add an imGuiIntegration to the world" };
		}

		if (!imGuiIntegrationRes->init(rendererRes->getRendererContext(), *windowRes))
		{
//			return { Level::Error, "Couldn't init imgui integration" };
		}

		component::RenderDrawData imGuiRenderDrawData
		{
			.command = vulkan_renderer::ImGuiIntegration::DrawCommand
		};

		world.spawn(imGuiRenderDrawData);
	}

	void ImGui::Update(core::ecs::World& world)
	{
		ImGuiIntegration::ImplSpecificNewFrame();

		::ImGui::NewFrame();

#	if ZINET_DEBUG
		::ImGui::ShowDemoWindow();
#	endif

		// TODO: Handle imgui calls here in some way like components with commands or use another systems

		::ImGui::EndFrame();

		auto imGuiIntegrationRes = world.getResource<ImGuiIntegration>();
		if (!imGuiIntegrationRes)
		{
//			return { Level::Error, "Couldn't get the imGuiIntegration from the world" };
		}

		imGuiIntegrationRes->prepareRenderData();
	}

	void ImGui::Deinit(core::ecs::World& world)
	{
		auto rendererRes = world.getResource<VulkanRenderer>();
		if (!rendererRes)
		{
//			return { Level::Error, "Couldn't get a renderer res from the world" };
		}

		auto imGuiIntegrationRes = world.getResource<ImGuiIntegration>();
		if (!imGuiIntegrationRes)
		{
//			return { Level::Error, "Couldn't get an imGuiIntegration to the world" };
		}

		imGuiIntegrationRes->deinit(rendererRes->getRendererContext());
	}

}