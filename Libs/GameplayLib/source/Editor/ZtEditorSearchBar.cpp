#include "Zinet/Gameplay/Editor/ZtEditorSearchBar.hpp"

#include <imgui.h>

namespace zt::gameplay
{
	std::string_view EditorSearchBar::show()
	{
		ImGui::InputText("Search Bar", assetsListBuffer.data(), assetsListBuffer.size());
		return std::string_view{ assetsListBuffer.begin(), std::ranges::find(assetsListBuffer, '\0') };
	}
}