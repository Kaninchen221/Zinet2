﻿#include "Zinet/Gameplay/Systems/ZtSystemImGui.hpp"
#include "Zinet/Gameplay/ZtEngineContext.hpp"

#include <imgui.h>

#include "Zinet/VulkanRenderer/ZtImGuiIntegration.hpp"

namespace zt::gameplay
{
	bool SystemImGui::init() ZINET_API_POST
	{
		System::init();
		return true;
	}

	void SystemImGui::deinit() ZINET_API_POST
	{
		System::deinit();
	}

	void SystemImGui::update() ZINET_API_POST
	{
		System::update();

		using namespace zt::vulkan_renderer;
		ImGuiIntegration::ImplSpecificNewFrame();

		ImGui::NewFrame();

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