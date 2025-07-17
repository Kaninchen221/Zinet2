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

		System() ZINET_API_POST = default;
		System(const System& other) ZINET_API_POST = default;
		System(System&& other) ZINET_API_POST = default;
		~System() ZINET_API_POST = default;

		System& operator = (const System& other) ZINET_API_POST = default;
		System& operator = (System&& other) ZINET_API_POST = default;

		virtual bool init() ZINET_API_POST { return true; }

		virtual void deinit() ZINET_API_POST { }

		virtual void addNode(const NodeWeakHandle<NodeT>& node) ZINET_API_POST { nodes.push_back(node); }

		auto& getNodes() ZINET_API_POST { return nodes; }
		const auto& getNodes() const ZINET_API_POST { return nodes; }

	protected:

		NodesT nodes;

	};

}