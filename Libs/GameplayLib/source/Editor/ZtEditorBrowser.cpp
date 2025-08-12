#include "Zinet/Gameplay/Editor/ZtEditorBrowser.hpp"

#include "Zinet/Gameplay/Editor/ZtEditorConfig.hpp"

namespace zt::gameplay
{
	void EditorBrowserInspector::show(core::Object* object)
	{
		const ImVec2 size = {};
		if (ImGui::BeginChild("Inspector", size, true))
		{
			if (object)
			{
				object->imGui();
			}

		}
		ImGui::EndChild();
	}

	void CreateObjectBrowserListElement(core::Object& object, EditorBrowserList& list)
	{
		const auto& displayName = object.getDisplayName();
		bool isSelected = &object == list.selectedObject;
		if (ImGui::Selectable(displayName.c_str(), isSelected))
		{
			list.selectedObject = &object;
			EditorBrowserList::Logger->info("Selected: {}", displayName);
		}

		CreateDragDropSourceSection(object);
	}

	void CreateNodeBrowserListElement(gameplay::Node& node, EditorBrowserList& list)
	{
		const auto& displayName = node.getDisplayName();
		if (!node.isInspectable())
			return;

		bool isSelected = &node == list.selectedObject;
		if (ImGui::Selectable(displayName.c_str(), isSelected))
		{
			list.selectedObject = &node;
			EditorBrowserList::Logger->info("Selected: {}", displayName);
		}

		CreateDragDropSourceSection(node);

		ImGui::Indent(EditorConfig::IndentValue);
		for (auto& child : node.getChildren())
		{
			if (child)
				CreateNodeBrowserListElement(*child.get(), list);
		}
		ImGui::Unindent(EditorConfig::IndentValue);
	}
}