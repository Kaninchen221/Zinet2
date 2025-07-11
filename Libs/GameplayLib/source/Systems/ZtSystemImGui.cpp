#include "Zinet/Gameplay/Systems/ZtSystemImGui.hpp"
#include "Zinet/Gameplay/ZtEngineContext.hpp"

#include <imgui.h>

#include "Zinet/VulkanRenderer/ZtImGuiIntegration.hpp"

namespace zt::gameplay
{
	bool SystemImGui::init() ZINET_API_POST
	{
		return true;
	}

	void SystemImGui::deinit() ZINET_API_POST
	{
	}

	void SystemImGui::addNode(const NodeWeakHandle<NodeT>& node) ZINET_API_POST
	{
		ClassBaseT::addNode(node);
	}

	void SystemImGui::update() ZINET_API_POST
	{
		using namespace zt::vulkan_renderer;
		ImGuiIntegration::ImplSpecificNewFrame();

		ImGui::NewFrame();
		ImGui::ShowDemoWindow();

		for (auto& weakNodeHandle : nodes)
		{
			if (!weakNodeHandle.expired())
			{
				auto nodeHandle = weakNodeHandle.lock();
				nodeHandle->imGui();
			}
		}

		ImGui::EndFrame();
	}

}