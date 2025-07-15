#pragma once

#include "Zinet/Gameplay/ZtGameplayConfig.hpp"
#include "Zinet/Gameplay/Nodes/ZtNode.hpp"

#include "Zinet/Core/ZtLogger.hpp"

#include "Zinet/VulkanRenderer/ZtTransform.hpp"

namespace zt::gameplay
{
	class ZINET_GAMEPLAY_API Node2D : public Node
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("zt::gameplay::Node2D");

	public:

		Node2D() ZINET_API_POST = delete;
		Node2D(const NodeNameView& newName) ZINET_API_POST : Node{ newName } {}
		Node2D(const Node2D& other) ZINET_API_POST = default;
		Node2D(Node2D&& other) ZINET_API_POST = default;
		~Node2D() ZINET_API_POST = default;

		Node2D& operator = (const Node2D& other) ZINET_API_POST = default;
		Node2D& operator = (Node2D&& other) ZINET_API_POST = default;

		vulkan_renderer::
		Transform transform;

	};

}