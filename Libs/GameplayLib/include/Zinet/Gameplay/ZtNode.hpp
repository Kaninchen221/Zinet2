#pragma once

#include "Zinet/Gameplay/ZtGameplayConfig.hpp"

#include "Zinet/Core/ZtLogger.hpp"

namespace zt::gameplay
{
	class ZINET_GAMEPLAY_API Node
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("Node");

	public:

		Node() noexcept = default;
		Node(const Node& other) noexcept = default;
		Node(Node&& other) noexcept = default;
		~Node() noexcept = default;

		Node& operator = (const Node& other) noexcept = default;
		Node& operator = (Node&& other) noexcept = default;

		using Children = std::vector<std::shared_ptr<Node>>;
		const Children& getChildren() const noexcept { return children; }

		template<class NodeT>
		std::shared_ptr<Node> addNode() noexcept;

		auto begin() const noexcept { return children.begin(); }
		auto end() const noexcept { return children.end(); }

	protected:

		Children children;

	};

	template<class NodeT>
	std::shared_ptr<Node> Node::addNode() noexcept
	{
		return children.emplace_back(new NodeT{});
	}

}