#pragma once

#include "Zinet/Gameplay/ZtGameplayConfig.hpp"

#include "Zinet/Core/ZtLogger.hpp"
#include "Zinet/Core/ZtObject.hpp"

namespace zt::gameplay
{
	class Node;
	using NodeName = std::string;
	using NodeNameView = std::string_view;

	template<class NodeT = Node>
	using NodeHandle = std::shared_ptr<NodeT>;

	template<class NodeT = Node>
	using NodeWeakHandle = std::weak_ptr<NodeT>;

	template<class NodeT = Node>
	static auto CreateNode(const NodeNameView& name = "DefaultName") ZINET_API_POST;

	class ZINET_GAMEPLAY_API Node : public core::Object
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("Node");

	public:

		Node() ZINET_API_POST = delete;
		Node(const NodeNameView& newName) ZINET_API_POST { name = newName; }
		Node(const Node& other) ZINET_API_POST = default;
		Node(Node&& other) ZINET_API_POST = default;
		virtual ~Node() ZINET_API_POST = default;

		Node& operator = (const Node& other) ZINET_API_POST = default;
		Node& operator = (Node&& other) ZINET_API_POST = default;

		using Children = std::vector<NodeHandle<Node>>;
		auto& getChildren() ZINET_API_POST { return children; }
		const auto& getChildren() const ZINET_API_POST { return children; }

		template<class NodeHandleT>
		void addChild(NodeHandleT nodeHandle) ZINET_API_POST;

		auto begin() const ZINET_API_POST { return children.begin(); }
		auto end() const ZINET_API_POST { return children.end(); }

		void setParent(NodeWeakHandle<Node> newParent) ZINET_API_POST { parent = newParent; }
		auto getParent() const ZINET_API_POST { return parent; }

		virtual void imGui() ZINET_API_POST;

		const auto& getName() const { return name; }
		void setName(const NodeNameView& newName) { name = newName; }

		std::string getDisplayName() override ZINET_API_POST { return name; }

	protected:

		Children children;
		NodeWeakHandle<Node> parent;
		NodeName name;

	};

	template<class NodeT>
	auto CreateNode(const NodeNameView& name) ZINET_API_POST
	{
		auto node = new NodeT(name);
		auto nodeHandle = NodeHandle<NodeT>{ node };
		return nodeHandle;
	}

	template<class NodeHandleT>
	void Node::addChild(NodeHandleT nodeHandle) ZINET_API_POST
	{
		children.push_back(nodeHandle);
	}

}