#pragma once

#include "Zinet/VulkanRenderer/ZtVulkanRendererConfig.hpp"

#include "Zinet/Math/ZtVecTypes.hpp"

#include <array>

namespace zt::vulkan_renderer
{
	class ShaderModule;

	class Vertex
	{
	public:

		Vector3f position;
		Vector4f color;
		Vector2f uv;

		auto operator<=>(const Vertex&) const = default;
	};

	using InputBindingDescription = VkVertexInputBindingDescription;
	 InputBindingDescription GetVertexInputBindingDescription() noexcept;

	using InputAttributesDescriptions = std::array<VkVertexInputAttributeDescription, 3>;
	 InputAttributesDescriptions GetVertexInputAttributesDescriptions() noexcept;
}