#include "Zinet/Gameplay/Systems/ZtSystemImGui.hpp"

#include "Zinet/Core/ECS/ZtWorld.hpp"

#include "Zinet/VulkanRenderer/ZtImGuiIntegration.hpp"
#include "Zinet/VulkanRenderer/ZtVulkanRenderer.hpp"

#include "Zinet/Window/ZtWindow.hpp"

using namespace zt::vulkan_renderer;

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

		auto rendererRes = world.getResource<VulkanRenderer>();
		if (!rendererRes)
		{
			Logger->error("Couldn't get a renderer res from the world");
			return;
		}

		auto imGuiIntegrationRes = world.addResource(ImGuiIntegration{});
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

		{
			GraphicsPipelineCreateInfo createInfo
			{
				rendererRes->getRendererContext()
			};

			// TODO: To create minimal graphics pipeline we need at least some really simple vertex and fragment shaders
			GraphicsPipeline graphicsPipeline;
			graphicsPipeline.create(createInfo);

			world.spawn(graphicsPipeline);
		}
	}

	void ImGui::Deinit(core::ecs::World& world)
	{
		auto rendererRes = world.getResource<VulkanRenderer>();
		if (!rendererRes)
		{
			Logger->error("Couldn't get a renderer res from the world");
			return;
		}

		auto imGuiIntegrationRes = world.getResource<ImGuiIntegration>();
		if (!imGuiIntegrationRes)
		{
			Logger->error("Couldn't get an imGuiIntegration to the world");
			return;
		}

		imGuiIntegrationRes->deinit(rendererRes->getRendererContext());
	}
}