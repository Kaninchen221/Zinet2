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
	static auto CreateNode() noexcept;

	class ZINET_GAMEPLAY_API Node
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("Node");

	public:

		Node() noexcept = default;
		Node(const Node& other) noexcept = default;
		Node(Node&& other) noexcept = default;
		virtual ~Node() noexcept = default;

		Node& operator = (const Node& other) noexcept = default;
		Node& operator = (Node&& other) noexcept = default;

		using Children = std::vector<std::shared_ptr<Node>>;
		const Children& getChildren() const noexcept { return children; }

		template<class NodeHandleT>
		void addChild(NodeHandleT nodeHandle) noexcept;

		auto begin() const noexcept { return children.begin(); }
		auto end() const noexcept { return children.end(); }

		void setParent(NodeWeakHandle<Node> newParent) noexcept { parent = newParent; }
		auto getParent() const noexcept { return parent; }

		virtual void imGui() noexcept {}

	protected:

		Children children;
		NodeWeakHandle<Node> parent;

	};

	template<class NodeT>
	auto CreateNode() noexcept
	{
		return NodeHandle<NodeT>{ new NodeT{} };
	}

	template<class NodeHandleT>
	void Node::addChild(NodeHandleT nodeHandle) noexcept
	{
		children.push_back(nodeHandle);
	}

}