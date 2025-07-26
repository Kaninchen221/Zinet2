#include "Zinet/Gameplay/Editor/ZtEditorBrowser.hpp"

namespace zt::gameplay
{

	void CreateObjectBrowserListElement(core::Object& object, EditorBrowserList& list, int elementIndex)
	{
		const auto& displayName = object.getDisplayName();
		bool isSelected = elementIndex == list.selectedIndex;
		if (ImGui::Selectable(displayName.c_str(), isSelected))
		{
			list.selectedIndex = elementIndex;
			EditorBrowserList::Logger->info("Selected: {} Index: {}", displayName, list.selectedIndex);
		}

		CreateDragDropSourceSection(object);
	}

}