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

		template<class NodeT = Node>
		static auto CreateNode() noexcept;

		using Children = std::vector<std::shared_ptr<Node>>;
		const Children& getChildren() const noexcept { return children; }

		template<class NodeHandleT>
		void addNode(NodeHandleT nodeHandle) noexcept;

		auto begin() const noexcept { return children.begin(); }
		auto end() const noexcept { return children.end(); }

	protected:

		Children children;

	};

	template<class NodeT>
	auto Node::CreateNode() noexcept
	{
		return NodeHandle<NodeT>{ new NodeT{} };
	}

	template<class NodeHandleT>
	void Node::addNode(NodeHandleT nodeHandle) noexcept
	{
		children.push_back(nodeHandle);
	}

}