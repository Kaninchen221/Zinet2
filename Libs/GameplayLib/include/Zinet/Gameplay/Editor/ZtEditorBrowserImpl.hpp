#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"

#include <imgui.h>

namespace zt::gameplay
{
	template<class ObjectT>
	void CreateDragDropSourceSection(ObjectT& object) ZINET_API_POST
	{
		if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
		{
			void* payloadData = &object;
			ImGui::SetDragDropPayload(ZinetImGuiPayloadType, &payloadData, sizeof(void*));
			ImGui::Text(object.getDisplayName().c_str());
			ImGui::EndDragDropSource();
		}
	}

	template<class ContainerT>
	void EditorBrowserList::show(ContainerT& container, std::string_view searchText, auto elementCreator) ZINET_API_POST
	{
		const ImVec2 size = {};
		const bool borders = true;

		if (ImGui::BeginChild("List", size, borders))
		{
			int elementIndex = 0;
			for (auto& containerElement : container)
			{
				auto& object = ResolveOptionalSmartPointer<typename ContainerT::value_type>(containerElement);
				const auto& displayName = object.getDisplayName();

				if (!searchText.empty() && !displayName.contains(searchText))
					continue;

				ImGui::PushID(elementIndex);
				std::invoke(elementCreator, object, *this);
				ImGui::PopID();

				++elementIndex;
			}

		}
		ImGui::EndChild();
	}

	template<class ContainerT>
	void EditorBrowser::show(ContainerT& container, auto BrowserListElementCreator) ZINET_API_POST
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
				list.show(container, searchText, BrowserListElementCreator);
				ImGui::TableNextColumn();
				inspector.show(list.selectedObject);

				ImGui::EndTable();
			}
		}
		ImGui::End();
	}
}