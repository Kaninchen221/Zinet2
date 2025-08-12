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
		void show();
	};

	class ZINET_GAMEPLAY_API NodeEditor : public Node
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("zt::gameplay::NodeEditor");

	public:

		NodeEditor() : Node{} { isInspectable = false; }
		NodeEditor(const NodeEditor& other) = default;
		NodeEditor(NodeEditor&& other) noexcept = default;
		~NodeEditor() noexcept = default;

		NodeEditor& operator = (const NodeEditor& other) = default;
		NodeEditor& operator = (NodeEditor&& other) noexcept = default;

		void imGui() override;

	protected:

		void showToolsMenu();

		EditorBrowser assetsList{ "Assets List" };
		EditorMetrics metrics;
		EditorBrowser nodesList{ "Nodes List" };
		EditorBrowser systemsList{ "Systems List" };
	};

}