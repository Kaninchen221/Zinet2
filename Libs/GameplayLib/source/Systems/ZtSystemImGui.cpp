#include "Zinet/Gameplay/Systems/ZtSystemImGui.hpp"

#include "Zinet/Gameplay/Components/ZtRenderDrawData.hpp"

#include "Zinet/Core/ECS/ZtWorld.hpp"

#include "Zinet/Core/ZtExitReason.hpp"

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
			worldCommands.addResource(ExitReason{ "Invalid window res"});
			return;
		}

		if (!rendererRes)
		{
			worldCommands.addResource(ExitReason{ "Invalid renderer res" });
			return;
		}

		// TODO: Refactor this fix
		// Fix github actions
		int monitorsCount = 0;
		glfwGetMonitors(&monitorsCount);
		const bool useMultiViewport = monitorsCount > 0;

		ImGuiIntegration imGuiIntegration;
		if (!imGuiIntegration.init(rendererRes->getRendererContext(), *windowRes, useMultiViewport))
		{
			worldCommands.addResource(ExitReason{ "Couldn't init imGui integration" });
			return;
		}

		worldCommands.addResource(std::move(imGuiIntegration));
		
		component::RenderDrawData imGuiRenderDrawData
		{
			.command = vulkan_renderer::ImGuiIntegration::DrawCommand
		};

		worldCommands.spawn(imGuiRenderDrawData, ImGui{});

		worldCommands.addResource(ImGuiData{}); // TODO: Test it
	}

	void ImGui::PreUpdate(
		ecs::Resource<ImGuiData> imGuiData,
		ecs::Query<component::RenderDrawData, ImGui> imGuiRenderDrawDataQuery,
		ecs::ConstResource<wd::Window> windowRes)
	{
		const bool isWindowMinimized = windowRes->isMinimized();
		const bool isWindowOpen = windowRes->isOpen();
		const bool shouldDrawImGui = !isWindowMinimized && isWindowOpen;

		imGuiData->skipImGui = isWindowMinimized || !isWindowOpen;

#	if ZINET_SANITY_CHECK
		// We expect only one render draw data for imgui
		// 2 components because we expect imgui component as a filter for Query
		if (imGuiRenderDrawDataQuery.getComponentsCount() != 2)
		{
			Logger->error("We expect only one render draw data for imgui");
			return;
		}
#	endif

		for (auto [renderDrawData, imgui] : imGuiRenderDrawDataQuery)
		{
			renderDrawData->shouldDraw = shouldDrawImGui;
		}

		if (!windowRes->isOpen())
			return;

		ImGuiIntegration::ImplSpecificNewFrame();

		::ImGui::NewFrame();
	}

	void ImGui::PostUpdate(
		ecs::WorldCommands worldCommands,
		ecs::ConstResource<wd::Window> windowRes,
		ecs::ConstResource<vulkan_renderer::ImGuiIntegration> imGuiIntegrationRes)
	{
		if (!windowRes->isOpen())
			return;

		::ImGui::EndFrame();

		// Update additional Platform Windows
		auto& io = ::ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			::ImGui::UpdatePlatformWindows();
		}

		if (!imGuiIntegrationRes)
		{
			worldCommands.addResource(ExitReason{ "imGuiIntegration res is invalid" });
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
			worldCommands.addResource(ExitReason{ "Renderer res is invalid" });
			return;
		}

		if (!imGuiIntegrationRes)
		{
			worldCommands.addResource(ExitReason{ "imGuiIntegration res is invalid" });
			return;
		}

		imGuiIntegrationRes->deinit(rendererRes->getRendererContext());
	}

}