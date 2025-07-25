#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"

#include <imgui.h>

namespace zt::gameplay
{
	template<class ContainerT>
	void EditorBrowserInspector::show(ContainerT& container, int selectedIndex) ZINET_API_POST
	{
		const ImVec2 size = {};
		if (ImGui::BeginChild("Inspector", size, true))
		{
			if (selectedIndex != InvalidIndex)
			{
				constexpr bool isSmartPointer = IsSmartPointer<typename ContainerT::value_type>();
				if constexpr (isSmartPointer)
				{
					container.at(selectedIndex)->imGui();
				}
				else
				{
					container.at(selectedIndex).imGui();
				}
			}

		}
		ImGui::EndChild();
	}

	template<class ContainerT>
	void EditorBrowserList::show(ContainerT& container, std::string_view searchText) ZINET_API_POST
	{
		const ImVec2 size = {};
		const bool borders = true;

		if (ImGui::BeginChild("List", size, borders))
		{
			int selectableIndex = 0;
			for (auto& object : container)
			{
				std::string displayName;
				constexpr bool isSmartPointer = IsSmartPointer<typename ContainerT::value_type>();
				if constexpr (isSmartPointer)
				{
					if (object)
					{
						displayName = object->getDisplayName();
					}
					else
					{
						Logger->warn("Found invalid smart pointer");
						continue;
					}
				}
				else
				{
					displayName = object.getDisplayName();
				}

				if (!searchText.empty() && !displayName.contains(searchText))
					continue;

				ImGui::PushID(selectableIndex);
				bool isSelected = selectableIndex == selectedIndex;
				if (ImGui::Selectable(displayName.c_str(), isSelected))
				{
					selectedIndex = selectableIndex;
					Logger->info("Selected: {} Index: {}", displayName, selectedIndex);
				}

				if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
				{
					void* payloadData;
					if (isSmartPointer)
						payloadData = object.get();
					else
						payloadData = &object;

					ImGui::SetDragDropPayload(ZinetImGuiPayloadType, &payloadData, sizeof(void*));
					ImGui::Text(displayName.c_str());
					ImGui::EndDragDropSource();
				}
				ImGui::PopID();

				++selectableIndex;
			}

		}
		ImGui::EndChild();
	}

	template<class ContainerT>
	void EditorBrowser::show(ContainerT& container) ZINET_API_POST
	{
		if (ImGui::Begin(title.c_str(), &isOpen))
		{
			const auto searchText = searchBar.show();
			ImGui::Separator();
			const int columns = 2;
			ImGuiTableFlags flags = ImGuiTableFlags_Resizable;
			if (ImGui::BeginTable("EditorBrowser", columns, flags))
			{
				ImGui::TableSetupColumn("List", ImGuiTableColumnFlags_WidthFixed);
				ImGui::TableSetupColumn("Inspector", ImGuiTableFlags_SizingStretchProp);

				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				list.show(container, searchText);
				ImGui::TableNextColumn();
				inspector.show(container, list.selectedIndex);

				ImGui::EndTable();
			}
		}
		ImGui::End();
	}
}