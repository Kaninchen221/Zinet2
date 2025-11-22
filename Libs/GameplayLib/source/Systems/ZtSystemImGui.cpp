#include "Zinet/Gameplay/Systems/ZtSystemImGui.hpp"

#include "Zinet/Gameplay/Components/ZtRenderDrawData.hpp"

#include "Zinet/Core/ECS/ZtWorld.hpp"

#include "Zinet/Core/Components/ZtExitReason.hpp"

#include "Zinet/VulkanRenderer/ZtImGuiIntegration.hpp"
#include "Zinet/VulkanRenderer/ZtVulkanRenderer.hpp"

#include "Zinet/Window/ZtWindow.hpp"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>

using namespace zt::vulkan_renderer;
using namespace zt::core;

namespace zt::gameplay::system
{
	void ImGui::Init(
		ecs::WorldCommands worldCommands,
		ecs::Resource<wd::Window> windowRes,
		ecs::ConstResource<vulkan_renderer::VulkanRenderer> rendererRes)
	{
		if (!windowRes)
		{
			worldCommands.addResource(ExitReason{ true, "Invalid window res"});
			return;
		}

		if (!rendererRes)
		{
			worldCommands.addResource(ExitReason{ true, "Invalid renderer res" });
			return;
		}

		ImGuiIntegration imGuiIntegration;
		if (!imGuiIntegration.init(rendererRes->getRendererContext(), *windowRes))
		{
			worldCommands.addResource(ExitReason{ true, "Couldn't init imGui integration" });
			return;
		}

		worldCommands.addResource(std::move(imGuiIntegration));
		
		component::RenderDrawData imGuiRenderDrawData
		{
			.command = vulkan_renderer::ImGuiIntegration::DrawCommand
		};

		worldCommands.spawn(std::move(imGuiRenderDrawData));
	}

	void ImGui::Update(
		core::ecs::WorldCommands worldCommands,
		core::ecs::ConstResource<vulkan_renderer::ImGuiIntegration> imGuiIntegrationRes)
	{
		ImGuiIntegration::ImplSpecificNewFrame();

		::ImGui::NewFrame();

#	if ZINET_DEBUG
		::ImGui::ShowDemoWindow();
#	endif

		// TODO: Handle imgui calls here in some way like components with commands or use another systems

		::ImGui::EndFrame();

		if (!imGuiIntegrationRes)
		{
			worldCommands.addResource(ExitReason{ true, "imGuiIntegration res is invalid" });
			return;
		}

 		imGuiIntegrationRes->prepareRenderData();
	}

	void ImGui::Deinit(
		ecs::WorldCommands worldCommands,
		ecs::ConstResource<vulkan_renderer::VulkanRenderer> rendererRes,
		ecs::Resource<vulkan_renderer::ImGuiIntegration> imGuiIntegrationRes)
	{
		if (!rendererRes)
		{
			worldCommands.addResource(ExitReason{ true, "Renderer res is invalid" });
			return;
		}

		if (!imGuiIntegrationRes)
		{
			worldCommands.addResource(ExitReason{ true, "imGuiIntegration res is invalid" });
			return;
		}

		imGuiIntegrationRes->deinit(rendererRes->getRendererContext());
	}

}