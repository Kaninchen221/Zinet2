#pragma once

#include "Zinet/Gameplay/ZtGameplayConfig.hpp"
#include "Zinet/Gameplay/ZtNode.hpp"

#include "Zinet/Core/ZtLogger.hpp"

namespace zt::gameplay
{
	template<class NodeType = Node>
	class System
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("zt::gameplay::System");

	public:

		using NodeT = NodeType;
		using NodesT = std::vector<NodeWeakHandle<NodeT>>;

		System() noexcept = default;
		System(const System& other) noexcept = default;
		System(System&& other) noexcept = default;
		~System() noexcept = default;

		System& operator = (const System& other) noexcept = default;
		System& operator = (System&& other) noexcept = default;

		void addNode(const NodeWeakHandle<NodeT>& node) noexcept { nodes.push_back(node); }

		auto& getNodes() noexcept { return nodes; }
		const auto& getNodes() const noexcept { return nodes; }

	protected:

		NodesT nodes;

	};

}