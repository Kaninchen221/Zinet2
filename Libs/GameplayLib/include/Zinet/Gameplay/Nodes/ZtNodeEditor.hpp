#pragma once

#include "Zinet/Gameplay/ZtGameplayConfig.hpp"
#include "Zinet/Gameplay/Nodes/ZtNode.hpp"
#include "Zinet/Gameplay/Editor/ZtEditorBrowser.hpp"
#include "Zinet/Gameplay/Editor/ZtEditorSearchBar.hpp"

#include "Zinet/Core/ZtLogger.hpp"
#include "Zinet/Core/ZtObject.hpp"
#include "Zinet/Core/Assets/ZtAssetsStorage.hpp"

namespace zt::gameplay
{
	struct EditorMetrics
	{
		bool shouldShow = false;
		void show() ZINET_API_POST;
	};

	class ZINET_GAMEPLAY_API NodeEditor : public Node
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("zt::gameplay::NodeEditor");

	public:

		NodeEditor() ZINET_API_POST = delete;
		NodeEditor(const std::string_view& newName) ZINET_API_POST : Node{ newName } { isInspectable = false; }
		NodeEditor(const NodeEditor& other) ZINET_API_POST = default;
		NodeEditor(NodeEditor&& other) ZINET_API_POST = default;
		~NodeEditor() ZINET_API_POST = default;

		NodeEditor& operator = (const NodeEditor& other) ZINET_API_POST = default;
		NodeEditor& operator = (NodeEditor&& other) ZINET_API_POST = default;

		void imGui() ZINET_API_POST override;

	protected:

		void showToolsMenu() ZINET_API_POST;

		EditorBrowser assetsList{ "Assets List" };
		EditorMetrics metrics;
		EditorBrowser nodesList{ "Nodes List" };
		EditorBrowser systemsList{ "Systems List" };
	};

}