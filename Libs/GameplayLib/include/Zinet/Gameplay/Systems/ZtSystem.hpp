#pragma once

#include "Zinet/Gameplay/ZtGameplayConfig.hpp"
#include "Zinet/Gameplay/Nodes/ZtNode.hpp"

#include "Zinet/Core/ZtLogger.hpp"

namespace zt::gameplay
{
	class EngineContext;

	template<class NodeType = Node>
	class System
	{

	public:

		using NodeT = NodeType;
		using NodesT = std::vector<NodeWeakHandle<NodeT>>;

		System() noexcept = default;
		System(const System& other) noexcept = default;
		System(System&& other) noexcept = default;
		~System() noexcept = default;

		System& operator = (const System& other) noexcept = default;
		System& operator = (System&& other) noexcept = default;

		virtual bool init() noexcept { return true; }

		virtual void deinit() noexcept { }

		virtual void addNode(const NodeWeakHandle<NodeT>& node) noexcept { nodes.push_back(node); }

		auto& getNodes() noexcept { return nodes; }
		const auto& getNodes() const noexcept { return nodes; }

	protected:

		NodesT nodes;

	};

}