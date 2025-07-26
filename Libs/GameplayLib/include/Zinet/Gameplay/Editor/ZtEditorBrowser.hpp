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

	template<class T>
	concept LikeSmartPointer = requires(T& t) {
		{ t.operator ->() };
		{ t.operator bool() };
	};

	template<class T>
	concept NotLikeSmartPointer = !LikeSmartPointer<T>;

	// TODO: Move it to concepts
	template<class T>
	concept LikePair = requires(T& t) {
		{ t.first } -> std::same_as<decltype(t.first)>;
		{ t.second } -> std::same_as<decltype(t.first)>;
	};

	template<class T>
	concept NotLikePair = !LikePair<T>;

	template<LikeSmartPointer InputT>
	constexpr auto& ResolveOptionalSmartPointer(InputT& object) ZINET_API_POST { return *object; }

	template<NotLikeSmartPointer InputT>
	constexpr auto& ResolveOptionalSmartPointer(InputT& object) ZINET_API_POST { return object; }

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

	struct EditorBrowserList;
	void CreateObjectBrowserListElement(core::Object& object, EditorBrowserList& list, int elementIndex);

	template<class ObjectT>
	void CreateDragDropSourceSection(ObjectT& object) ZINET_API_POST;

	struct EditorBrowserList
	{
		inline static auto Logger = core::ConsoleLogger::Create("zt::gameplay::EditorBrowserList");

		template<class ContainerT>
		void show(ContainerT& container, std::string_view searchText, auto ElementCreator) ZINET_API_POST;

		int selectedIndex = InvalidIndex;
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