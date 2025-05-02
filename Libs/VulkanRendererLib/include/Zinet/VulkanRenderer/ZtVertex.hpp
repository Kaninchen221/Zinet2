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
		Vector3f color;

		auto operator<=>(const Vertex&) const = default;

		using InputBindingDescription = VkVertexInputBindingDescription;
		static InputBindingDescription GetInputBindingDescription() noexcept;

		using InputAttributesDescriptions = std::array<VkVertexInputAttributeDescription, 2>;
		static InputAttributesDescriptions GetInputAttributesDescriptions() noexcept;
	};
}