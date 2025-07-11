#pragma once

#include "Zinet/Gameplay/ZtGameplayConfig.hpp"

#include "Zinet/Core/ZtLogger.hpp"

namespace zt::gameplay
{
	class Node;

	template<class NodeT = Node>
	using NodeHandle = std::shared_ptr<NodeT>;

	template<class NodeT = Node>
	using NodeWeakHandle = std::weak_ptr<NodeT>;

	template<class NodeT = Node>
	static auto CreateNode() ZINET_API_POST;

	class ZINET_GAMEPLAY_API Node
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("Node");

	public:

		Node() ZINET_API_POST = default;
		Node(const Node& other) ZINET_API_POST = default;
		Node(Node&& other) ZINET_API_POST = default;
		virtual ~Node() ZINET_API_POST = default;

		Node& operator = (const Node& other) ZINET_API_POST = default;
		Node& operator = (Node&& other) ZINET_API_POST = default;

		using Children = std::vector<std::shared_ptr<Node>>;
		const Children& getChildren() const ZINET_API_POST { return children; }

		template<class NodeHandleT>
		void addChild(NodeHandleT nodeHandle) ZINET_API_POST;

		auto begin() const ZINET_API_POST { return children.begin(); }
		auto end() const ZINET_API_POST { return children.end(); }

		void setParent(NodeWeakHandle<Node> newParent) ZINET_API_POST { parent = newParent; }
		auto getParent() const ZINET_API_POST { return parent; }

		virtual void imGui() ZINET_API_POST {}

	protected:

		Children children;
		NodeWeakHandle<Node> parent;

	};

	template<class NodeT>
	auto CreateNode() ZINET_API_POST
	{
		return NodeHandle<NodeT>{ new NodeT{} };
	}

	template<class NodeHandleT>
	void Node::addChild(NodeHandleT nodeHandle) ZINET_API_POST
	{
		children.push_back(nodeHandle);
	}

}