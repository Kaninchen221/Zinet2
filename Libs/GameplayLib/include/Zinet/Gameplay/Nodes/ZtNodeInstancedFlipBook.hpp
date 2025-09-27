#pragma once

#include "Zinet/Gameplay/Nodes/ZtNode.hpp"

#include "Zinet/Core/ZtLogger.hpp"

#include "Zinet/Math/ZtMVP.hpp"

#include "Zinet/VulkanRenderer/ZtTransform.hpp"

namespace zt::gameplay
{
	class SystemFlipBooks;

	struct FlipBookFrame
	{
		using Coords = std::array<Vector2f, 4>;
		Coords textureCoords;
		float time = 1.f; // Seconds 
		// TODO: Create literals for seconds, milliseconds etc.
		// Change it to core::Time
	};

	using FlipBookFrames = std::vector<FlipBookFrame>;

	class NodeInstancedFlipBook : public Node
	{
		friend SystemFlipBooks;

	public:

		NodeInstancedFlipBook() = default;
		NodeInstancedFlipBook(const NodeInstancedFlipBook& other) = default;
		NodeInstancedFlipBook(NodeInstancedFlipBook&& other) noexcept = default;
		~NodeInstancedFlipBook() noexcept = default;

		NodeInstancedFlipBook& operator = (const NodeInstancedFlipBook& other) = default;
		NodeInstancedFlipBook& operator = (NodeInstancedFlipBook&& other) noexcept = default;

		auto getID() const noexcept { return id; }

		vulkan_renderer::Transform& getTransform() noexcept;
		const vulkan_renderer::Transform& getTransform() const noexcept;

		FlipBookFrames& getFrames() noexcept;
		const FlipBookFrames& getFrames() const noexcept;

	protected:

		ObjectWeakHandle<SystemFlipBooks> systemFlipBooks;

		uint32_t id = 0;



	};
}