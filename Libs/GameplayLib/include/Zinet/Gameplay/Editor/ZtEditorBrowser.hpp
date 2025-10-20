#pragma once

#include "Zinet/Gameplay/ZtGameplayConfig.hpp"
#include "Zinet/Gameplay/Editor/ZtEditorSearchBar.hpp"

#include "Zinet/Core/ZtLogger.hpp"
#include "Zinet/Core/ZtObject.hpp"
#include "Zinet/Core/ZtConcepts.hpp"
#include "Zinet/Core/ZtImgui.hpp"

#include "Zinet/Core/Assets/ZtAssetsStorage.hpp"

namespace zt::gameplay
{
	template<core::LikeSmartPointer InputT>
	constexpr auto& ResolveOptionalSmartPointer(InputT& object) noexcept { return *object; }

	template<core::NotLikeSmartPointer InputT>
	constexpr auto& ResolveOptionalSmartPointer(InputT& object) noexcept { return object; }

	struct AbstractObject
	{
		std::string displayName = "name";

		const std::string& getDisplayName() { return displayName; }

		void show() { ImGui::Text(getDisplayName()); }
	};

	struct  EditorBrowserInspector
	{
		inline static auto Logger = core::ConsoleLogger::Create("zt::gameplay::EditorBrowserInspector");

		void show(ObjectHandle<core::Object>& object);
	};

	struct EditorBrowserList;

	template<class ObjectHandleLikeT>
	void CreateObjectBrowserListElement(ObjectHandleLikeT& object, EditorBrowserList& list);

	template<class ObjectHandleT>
	void CreateDragDropSourceSection(ObjectHandleT& object);

	struct EditorBrowserList
	{
		inline static auto Logger = core::ConsoleLogger::Create("zt::gameplay::EditorBrowserList");

		template<class ContainerT>
		void show(ContainerT& container, std::string_view searchText, auto ElementCreator);

		ObjectHandle<core::Object> selectedObject;
	};

	struct EditorBrowser
	{
		inline static auto Logger = core::ConsoleLogger::Create("zt::gameplay::EditorBrowser");

		template<class ContainerT>
		void show(ContainerT& container, auto BrowserListElementCreator = CreateDefaultBrowserListElement);

		// Config
		std::string title = "Editor Browser";

		// Data
		EditorBrowserList list;
		EditorBrowserInspector inspector;
		EditorSearchBar searchBar;
		bool isOpen = false;
	};

	template<class ObjectHandleLikeT>
	void CreateObjectBrowserListElement(ObjectHandleLikeT& object, EditorBrowserList& list)
	{
		const auto& displayName = object->getDisplayName();
		bool isSelected = object == list.selectedObject;
		if (ImGui::Selectable(displayName.c_str(), isSelected))
		{
			list.selectedObject = object;
			EditorBrowserList::Logger->info("Selected: {}", displayName);
		}
	
		CreateDragDropSourceSection(object);
	}
}

#include "Zinet/Gameplay/Editor/ZtEditorBrowserImpl.hpp"