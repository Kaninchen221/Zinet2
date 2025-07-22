#include "Zinet/Gameplay/Systems/ZtSystem.hpp"

#include <imgui.h>

namespace zt::gameplay
{
	void System::imGui() ZINET_API_POST
	{
		ImGui::Text(getName().c_str());
		ImGui::Text(fmt::format("Initialized: {}", initialized).c_str());
		ImGui::Separator();
	}
}