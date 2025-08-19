#include "Zinet/Gameplay/Systems/ZtSystemImGui.hpp"
#include "Zinet/Gameplay/ZtEngineContext.hpp"

#include <imgui.h>

#include "Zinet/VulkanRenderer/ZtImGuiIntegration.hpp"

namespace zt::gameplay
{
	bool SystemImGui::init()
	{
		return System::init();
	}

	bool SystemImGui::deinit()
	{
		return System::deinit();
	}

	void SystemImGui::update()
	{
		System::update();

		vulkan_renderer::
		ImGuiIntegration::ImplSpecificNewFrame();

		ImGui::NewFrame();

		for (auto& node : nodes)
		{
			if (node.isValid())
			{
				node->imGui();
			}
		}

		ImGui::EndFrame();
	}

}