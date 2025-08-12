#include "Zinet/Gameplay/Systems/ZtSystem.hpp"

#include <imgui.h>

namespace zt::gameplay
{
	void System::imGui()
	{
		ImGui::Text(getDisplayName().c_str());
		ImGui::Text(fmt::format("Initialized: {}", initialized).c_str());
		ImGui::Separator();
	}
}