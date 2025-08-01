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
	constexpr auto& ResolveOptionalSmartPointer(InputT& object) ZINET_API_POST { return *object; }

	template<core::NotLikeSmartPointer InputT>
	constexpr auto& ResolveOptionalSmartPointer(InputT& object) ZINET_API_POST { return object; }

	struct AbstractObject
	{
		std::string displayName = "name";

		const std::string& getDisplayName() ZINET_API_POST { return displayName; }

		void imGui() ZINET_API_POST { auto name = getDisplayName(); ImGui::Text(name.c_str()); }
	};

	struct ZINET_GAMEPLAY_API EditorBrowserInspector
	{
		inline static auto Logger = core::ConsoleLogger::Create("zt::gameplay::EditorBrowserInspector");

		void show(core::Object* object = nullptr) ZINET_API_POST;
	};

	struct EditorBrowserList;
	ZINET_GAMEPLAY_API void CreateObjectBrowserListElement(core::Object& object, EditorBrowserList& list);

	ZINET_GAMEPLAY_API void CreateNodeBrowserListElement(gameplay::Node& node, EditorBrowserList& list);

	template<class ObjectT>
	void CreateDragDropSourceSection(ObjectT& object) ZINET_API_POST;

	struct EditorBrowserList
	{
		inline static auto Logger = core::ConsoleLogger::Create("zt::gameplay::EditorBrowserList");

		template<class ContainerT>
		void show(ContainerT& container, std::string_view searchText, auto ElementCreator) ZINET_API_POST;

		core::Object* selectedObject = nullptr;
	};

	struct EditorBrowser
	{
		inline static auto Logger = core::ConsoleLogger::Create("zt::gameplay::EditorBrowser");

		template<class ContainerT>
		void show(ContainerT& container, auto BrowserListElementCreator = CreateDefaultBrowserListElement) ZINET_API_POST;

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