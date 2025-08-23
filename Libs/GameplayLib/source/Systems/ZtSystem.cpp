#include "Zinet/Gameplay/Systems/ZtSystem.hpp"

#include "Zinet/Core/ZtImgui.hpp"

namespace zt::gameplay
{
	void System::show()
	{
		ImGui::Text(getDisplayName());
		ImGui::TextFMT("Initialized: {}", initialized);
		ImGui::TextFMT("Nodes count: {}", nodes.size());
		ImGui::Separator();
	}
}