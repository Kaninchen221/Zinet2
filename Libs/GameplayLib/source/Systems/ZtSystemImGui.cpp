#include "Zinet/Gameplay/Systems/ZtSystemImGui.hpp"
#include "Zinet/Gameplay/ZtEngineContext.hpp"

#include <imgui.h>

#include "Zinet/VulkanRenderer/ZtImGuiIntegration.hpp"

namespace zt::gameplay
{
	bool SystemImGui::init([[maybe_unused]] EngineContext& engineContext) noexcept
	{
		return true;
	}

	void SystemImGui::deinit() noexcept
	{
	}

	void SystemImGui::addNode(const NodeWeakHandle<NodeT>& node) noexcept
	{
		ClassBaseT::addNode(node);
	}

	void SystemImGui::update() noexcept
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