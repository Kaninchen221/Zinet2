#pragma once

#include "Zinet/Gameplay/ZtGameplayConfig.hpp"
#include "Zinet/Gameplay/Nodes/ZtNode.hpp"

#include "Zinet/Core/ZtLogger.hpp"

namespace zt::gameplay
{
	class ZINET_GAMEPLAY_API NodeEditor : public Node
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("zt::gameplay::NodeEditor");

	public:

		NodeEditor() ZINET_API_POST = default;
		NodeEditor(const NodeEditor& other) ZINET_API_POST = default;
		NodeEditor(NodeEditor&& other) ZINET_API_POST = default;
		~NodeEditor() ZINET_API_POST = default;

		NodeEditor& operator = (const NodeEditor& other) ZINET_API_POST = default;
		NodeEditor& operator = (NodeEditor&& other) ZINET_API_POST = default;

		void imGui() ZINET_API_POST override;
	};

}