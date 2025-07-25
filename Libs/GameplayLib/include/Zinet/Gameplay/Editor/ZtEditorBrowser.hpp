#pragma once

#include "Zinet/Gameplay/ZtGameplayConfig.hpp"
#include "Zinet/Gameplay/Nodes/ZtNode.hpp"

#include "Zinet/Core/ZtLogger.hpp"
#include "Zinet/Core/ZtObject.hpp"
#include "Zinet/Core/Assets/ZtAssetsStorage.hpp"

#include "Zinet/Gameplay/Editor/ZtEditorSearchBar.hpp"

#include <imgui.h>

namespace zt::gameplay
{
	// TODO: Constants.hpp?
	const inline static int InvalidIndex = -1;

	// TODO: Move it to concepts
	template<class T>
	constexpr bool IsSmartPointer() 
	{
		return requires(T& t) { t.operator -> (); } &&
		requires(T& t) { t.operator bool(); };
	}

	struct AbstractObject
	{
		std::string displayName = "name";

		const std::string& getDisplayName() ZINET_API_POST { return displayName; }

		void imGui() ZINET_API_POST { auto name = getDisplayName(); ImGui::Text(name.c_str()); }
	};

	struct EditorBrowserInspector
	{
		inline static auto Logger = core::ConsoleLogger::Create("zt::gameplay::EditorBrowserInspector");

		template<class ContainerT>
		void show(ContainerT& container, int selectedIndex) ZINET_API_POST;
	};

	struct EditorBrowserList
	{
		inline static auto Logger = core::ConsoleLogger::Create("zt::gameplay::EditorBrowserList");

		template<class ContainerT>
		void show(ContainerT& container, std::string_view searchText) ZINET_API_POST;

		int selectedIndex = InvalidIndex;
	};

	struct EditorBrowser
	{
		inline static auto Logger = core::ConsoleLogger::Create("zt::gameplay::EditorBrowser");

		template<class ContainerT>
		void show(ContainerT& container) ZINET_API_POST;

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