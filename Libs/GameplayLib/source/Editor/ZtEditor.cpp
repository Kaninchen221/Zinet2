#include "Zinet/Gameplay/Editor/ZtEditor.hpp"

#include <imgui.h>

#include "Zinet/Window/ZtWindow.hpp"

using namespace zt::core;

namespace zt::gameplay
{
	void Editor::EntryPoint(ecs::ConstResource<wd::Window> windowRes)
	{
		if (!windowRes || !windowRes->isOpen())
			return;

		::ImGui::ShowDemoWindow();

		::ImGui::Begin("Test");

		::ImGui::End();
	}
}