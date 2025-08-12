#pragma once

#include "Zinet/Gameplay/ZtGameplayConfig.hpp"
#include "Zinet/Gameplay/Nodes/ZtNode.hpp"

#include "Zinet/Core/ZtLogger.hpp"
#include "Zinet/Core/ZtObject.hpp"
#include "Zinet/Core/ZtConcepts.hpp"

#include "Zinet/Core/Assets/ZtAssetsStorage.hpp"

#include "Zinet/Gameplay/Editor/ZtEditorSearchBar.hpp"

#include <imgui.h>

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

		void imGui() { auto name = getDisplayName(); ImGui::Text(name.c_str()); }
	};

	struct  EditorBrowserInspector
	{
		inline static auto Logger = core::ConsoleLogger::Create("zt::gameplay::EditorBrowserInspector");

		void show(core::Object* object = nullptr);
	};

	struct EditorBrowserList;
	 void CreateObjectBrowserListElement(core::Object& object, EditorBrowserList& list);

	 void CreateNodeBrowserListElement(gameplay::Node& node, EditorBrowserList& list);

	template<class ObjectT>
	void CreateDragDropSourceSection(ObjectT& object);

	struct EditorBrowserList
	{
		inline static auto Logger = core::ConsoleLogger::Create("zt::gameplay::EditorBrowserList");

		template<class ContainerT>
		void show(ContainerT& container, std::string_view searchText, auto ElementCreator);

		core::Object* selectedObject = nullptr;
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
}

#include "Zinet/Gameplay/Editor/ZtEditorBrowserImpl.hpp"