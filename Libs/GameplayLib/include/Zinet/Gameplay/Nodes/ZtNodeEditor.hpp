#pragma once

#include "Zinet/Gameplay/ZtGameplayConfig.hpp"
#include "Zinet/Gameplay/Nodes/ZtNode.hpp"

#include "Zinet/Core/ZtLogger.hpp"
#include "Zinet/Core/Assets/ZtAssetsStorage.hpp"

namespace
{
	namespace assets = zt::core::assets;
}

namespace zt::gameplay
{
	struct EditorTextSearchBar
	{
		std::string_view show() ZINET_API_POST;
		std::array<char, 1000> assetsListBuffer{ '\0' };
	};

	struct EditorAssetsList
	{
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
		std::string searchSubString;
	};

	struct EditorNodeInspector
	{
		void show(NodeHandle<> node) ZINET_API_POST;
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
		EditorNodeInspector nodeInspector;

	};

}