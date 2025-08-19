#pragma once

#include "Zinet/Gameplay/ZtGameplayConfig.hpp"
#include "Zinet/Gameplay/Nodes/ZtNode.hpp"

#include "Zinet/Core/ZtLogger.hpp"

#include "Zinet/VulkanRenderer/ZtTransform.hpp"
#include "Zinet/VulkanRenderer/ZtDrawInfo.hpp"

namespace zt::gameplay
{
	class  Node2D : public Node
	{
	private:

		inline static auto Logger = core::ConsoleLogger::Create("zt::gameplay::Node2D");

	public:

		Node2D() = default;
		Node2D(const Node2D& other) = default;
		Node2D(Node2D&& other) noexcept = default;
		~Node2D() noexcept = default;

		Node2D& operator = (const Node2D& other) = default;
		Node2D& operator = (Node2D&& other) noexcept = default;

		vulkan_renderer::DescriptorInfo getPipelineDescriptorInfos() const;
		vulkan_renderer::DescriptorInfo getDrawCallDescriptorInfos() const;

		vulkan_renderer::Transform transform{};

	};

}