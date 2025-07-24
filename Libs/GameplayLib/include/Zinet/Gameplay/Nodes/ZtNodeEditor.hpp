#pragma once

#include "Zinet/Gameplay/ZtGameplayConfig.hpp"
#include "Zinet/Gameplay/Nodes/ZtNode.hpp"

#include "Zinet/Core/ZtLogger.hpp"
#include "Zinet/Core/ZtObject.hpp"
#include "Zinet/Core/Assets/ZtAssetsStorage.hpp"

namespace
{
	namespace assets = zt::core::assets;
}

namespace zt::gameplay
{
	struct AbstractObject : public core::Object
	{
		std::string getDisplayName() override ZINET_API_POST { return "name"; }

		void imGui() override ZINET_API_POST;
	};

	struct EditorBrowserInspector
	{
		void show() ZINET_API_POST;
	};

	struct EditorBrowserList
	{
		void show() ZINET_API_POST;
	};

	struct EditorBrowser
	{
		void show() ZINET_API_POST;

		EditorBrowserList list;
		EditorBrowserInspector inspector;
	};

	struct EditorConfig
	{
		inline static float IndentMulti = 4.f;
		inline static float ListMenuWidthDiv = 4.f;
	};

	struct EditorTextSearchBar
	{
		std::string_view show() ZINET_API_POST;
		std::array<char, 1000> assetsListBuffer{ '\0' };
	};

	struct EditorAssetsList
	{
		inline static auto Logger = core::ConsoleLogger::Create("zt::gameplay::EditorAssetsList");

		bool shouldShow = false;
		void show() ZINET_API_POST;
		int selectedAssetIndex = -1;
		assets::AssetsStorage::AssetsKey selectedAssetKey;
		EditorTextSearchBar textSearchBar;
	};

	struct EditorMetrics
	{
		bool shouldShow = false;
		void show() ZINET_API_POST;
	};

	struct EditorNodesList
	{
		inline static auto Logger = core::ConsoleLogger::Create("zt::gameplay::EditorNodesList");

		bool shouldShow = false;
		void show() ZINET_API_POST;

		void generateSelectable(NodeHandle<>& node, int deep) ZINET_API_POST;

		NodeHandle<> selectedNode;
		EditorTextSearchBar textSearchBar;
		std::string_view searchSubString;
	};

	struct EditorSystemsList
	{
		inline static auto Logger = core::ConsoleLogger::Create("zt::gameplay::EditorSystemsList");

		bool shouldShow = false;
		void show() ZINET_API_POST;

		EditorTextSearchBar textSearchBar;
		std::string_view searchSubString;
		const int32_t invalidSystemIndex = -1;
		int32_t selectedSystemIndex = invalidSystemIndex;
	};

	class ZINET_GAMEPLAY_API NodeEditor : public Node
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("zt::gameplay::NodeEditor");

	public:

		NodeEditor() ZINET_API_POST = delete;
		NodeEditor(const NodeNameView& newName) ZINET_API_POST : Node{ newName } {}
		NodeEditor(const NodeEditor& other) ZINET_API_POST = default;
		NodeEditor(NodeEditor&& other) ZINET_API_POST = default;
		~NodeEditor() ZINET_API_POST = default;

		NodeEditor& operator = (const NodeEditor& other) ZINET_API_POST = default;
		NodeEditor& operator = (NodeEditor&& other) ZINET_API_POST = default;

		void imGui() ZINET_API_POST override;

	protected:

		void showToolsMenu() ZINET_API_POST;

		EditorAssetsList assetsList;
		EditorMetrics metrics;
		EditorNodesList nodesList;
		EditorSystemsList systemsList;
		EditorBrowser editorBrowserTest;
	};

}