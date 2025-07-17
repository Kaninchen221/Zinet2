#pragma once

#include "Zinet/VulkanRenderer/ZtVulkanRendererConfig.hpp"

#include "Zinet/Math/ZtVecTypes.hpp"

#include <array>

namespace zt::vulkan_renderer
{
	class ShaderModule;

	class ZINET_VULKAN_RENDERER_API Vertex
	{
	public:

		Vector3f position;
		Vector4f color;
		Vector2f uv;

		auto operator<=>(const Vertex&) const = default;

		using InputBindingDescription = VkVertexInputBindingDescription;
		static InputBindingDescription GetInputBindingDescription() ZINET_API_POST;

		using InputAttributesDescriptions = std::array<VkVertexInputAttributeDescription, 3>;
		static InputAttributesDescriptions GetInputAttributesDescriptions() ZINET_API_POST;
	};
}