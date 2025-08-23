#include "Zinet/Gameplay/Editor/ZtEditorBrowser.hpp"

#include "Zinet/Gameplay/Editor/ZtEditorConfig.hpp"

namespace zt::gameplay
{
	void EditorBrowserInspector::show(ObjectHandle<core::Object>& object)
	{
		const ImVec2 size = {};
		if (ImGui::BeginChild("Inspector", size, true))
		{
			if (object)
			{
				object->show();
			}

		}
		ImGui::EndChild();
	}

	void CreateNodeBrowserListElement(ObjectHandle<Node>& node, EditorBrowserList& list)
	{
		const auto& displayName = node->getDisplayName();
		if (!node->isInspectable())
			return;

		bool isSelected = node == list.selectedObject;
		if (ImGui::Selectable(displayName.c_str(), isSelected))
		{
			list.selectedObject = node;
			EditorBrowserList::Logger->info("Selected: {}", displayName);
		}

		CreateDragDropSourceSection(node);

		ImGui::Indent(EditorConfig::IndentValue);
		for (auto& child : node->getChildren())
		{
			if (child)
				CreateNodeBrowserListElement(child, list);
		}
		ImGui::Unindent(EditorConfig::IndentValue);
	}

}