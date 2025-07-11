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

		NodeEditor() noexcept = default;
		NodeEditor(const NodeEditor& other) noexcept = default;
		NodeEditor(NodeEditor&& other) noexcept = default;
		~NodeEditor() noexcept = default;

		NodeEditor& operator = (const NodeEditor& other) noexcept = default;
		NodeEditor& operator = (NodeEditor&& other) noexcept = default;

		void imGui() noexcept override;
	};

}