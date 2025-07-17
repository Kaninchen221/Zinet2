#include "Zinet/Gameplay/Nodes/ZtNodeSprite.hpp"

#include <imgui.h>

namespace zt::gameplay
{
	void NodeSprite::imGuiNodeInspect() ZINET_API_POST
	{
		ImGui::Text("Texture:");
		ImGui::SameLine();
		if (texture)
		{
			auto textureName = texture->metaData.value("fileName", "fileName");
			ImGui::Text(textureName.c_str());
			const std::string isLoaded = fmt::format("Is loaded: {}", texture->isLoaded());
			ImGui::Text(isLoaded.c_str());
		}
		else
		{
			ImGui::Text("Invalid texture");
		}
	}
}