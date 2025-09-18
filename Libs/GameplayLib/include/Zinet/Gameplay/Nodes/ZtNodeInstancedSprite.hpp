#pragma once

#include "Zinet/Gameplay/Nodes/ZtNode.hpp"

#include "Zinet/Core/ZtLogger.hpp"

#include "Zinet/Math/ZtMVP.hpp"

#include "Zinet/VulkanRenderer/ZtTransform.hpp"

namespace zt::gameplay
{
	class SystemSprites;

	class NodeInstancedSprite : public Node
	{
		friend SystemSprites;

	public:

		NodeInstancedSprite() = default;
		NodeInstancedSprite(const NodeInstancedSprite& other) = default;
		NodeInstancedSprite(NodeInstancedSprite&& other) noexcept = default;
		~NodeInstancedSprite() noexcept = default;

		NodeInstancedSprite& operator = (const NodeInstancedSprite& other) = default;
		NodeInstancedSprite& operator = (NodeInstancedSprite&& other) noexcept = default;

		auto getID() const noexcept { return id; }

		vulkan_renderer::Transform& getTransform() noexcept;
		const vulkan_renderer::Transform& getTransform() const noexcept;

	protected:

		ObjectHandle<SystemSprites> systemSprites;

		uint32_t id = 0;



	};
}