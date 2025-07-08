#pragma once

#include "Zinet/Gameplay/ZtGameplayConfig.hpp"
#include "Zinet/Gameplay/ZtNode.hpp"

#include "Zinet/Core/ZtLogger.hpp"

#include "Zinet/VulkanRenderer/ZtTransform.hpp"

namespace zt::gameplay
{
	class ZINET_GAMEPLAY_API Node2D : public Node
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("zt::gameplay::Node2D");

	public:

		Node2D() noexcept = default;
		Node2D(const Node2D& other) noexcept = default;
		Node2D(Node2D&& other) noexcept = default;
		~Node2D() noexcept = default;

		Node2D& operator = (const Node2D& other) noexcept = default;
		Node2D& operator = (Node2D&& other) noexcept = default;

		vulkan_renderer::
		Transform transform;

	};

}