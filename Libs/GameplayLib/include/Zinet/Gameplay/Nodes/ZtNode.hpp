#pragma once

#include "Zinet/Gameplay/ZtGameplayConfig.hpp"

#include "Zinet/Core/ZtLogger.hpp"
#include "Zinet/Core/ZtObject.hpp"

namespace zt::gameplay
{
	class ZINET_GAMEPLAY_API Node : public core::Object
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

		using Children = std::vector<ObjectHandle<Node>>;
		auto& getChildren() ZINET_API_POST { return children; }
		const auto& getChildren() const ZINET_API_POST { return children; }

		template<class ObjectHandleT>
		void addChild(ObjectHandleT ObjectHandle) ZINET_API_POST;

		auto begin() const ZINET_API_POST { return children.begin(); }
		auto end() const ZINET_API_POST { return children.end(); }

		void setParent(ObjectWeakHandle<Node> newParent) ZINET_API_POST { parent = newParent; }
		auto getParent() const ZINET_API_POST { return parent; }

		virtual void imGui() ZINET_API_POST;

		virtual void update([[maybe_unused]] float deltaTime) ZINET_API_POST {}

	protected:

		Children children;
		ObjectWeakHandle<Node> parent;

	};

	template<class ObjectHandleT>
	void Node::addChild(ObjectHandleT ObjectHandle) ZINET_API_POST
	{
		children.push_back(ObjectHandle);
	}

}