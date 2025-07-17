#include "Zinet/Core/Assets/ZtAsset.hpp"

#include <fmt/format.h>
#include <imgui.h>

namespace zt::core::assets
{
	void Asset::imGuiAssetInspect() ZINET_API_POST
	{
		const std::string isLoadedText = fmt::format("Is loaded: {}", isLoaded());
		ImGui::Text(isLoadedText.c_str());

		if (ImGui::Button("Load")) { load(core::Paths::RootPath()); }
		ImGui::SameLine();
		if (ImGui::Button("Unload")) { unload(); }
		ImGui::Separator();

		if (ImGui::BeginTable("table1", 2 /*columns*/, {} /*flags*/))
		{
			for (auto& [key, value] : metaData.items())
			{
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::TextUnformatted(key.c_str());
				ImGui::TableSetColumnIndex(1);
				auto valueText = value.dump();
				ImGui::TextUnformatted(valueText.c_str());
			}

			ImGui::EndTable();
		}
	}

}