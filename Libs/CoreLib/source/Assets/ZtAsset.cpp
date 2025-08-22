#include "Zinet/Core/Assets/ZtAsset.hpp"

#include "Zinet/Core/ZtImgui.hpp"

namespace zt::core
{
	void Asset::imGui()
	{
		ImGui::TextFMT("Is loaded: {}", isLoaded());

		if (ImGui::Button("Load")) { load(core::Paths::RootPath()); }
		ImGui::SameLine();
		if (ImGui::Button("Unload")) { unload(); }
		ImGui::Separator();

		const ImGuiTableFlags tableFlags = {};
		if (ImGui::BeginTable("table1", 2 /*columns*/, tableFlags))
		{
			ImGui::TableSetupColumn("Property Name", ImGuiTableColumnFlags_WidthFixed);
			ImGui::TableSetupColumn("Property Value", ImGuiTableFlags_SizingStretchProp);

			for (auto& [key, value] : metaData.items())
			{
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::Text(key);
				ImGui::TableSetColumnIndex(1);
				auto valueText = value.dump();
				ImGui::Text(valueText);
			}

			ImGui::EndTable();
			ImGui::Separator();
		}
	}

}