#include "Zinet/VulkanRenderer/ZtVertex.hpp"

namespace zt::vulkan_renderer
{

	VkVertexInputBindingDescription Vertex::GetInputBindingDescription() ZINET_API_POST
	{
		return
		{
			.binding = 0,
			.stride = sizeof(Vertex),
			.inputRate = VK_VERTEX_INPUT_RATE_VERTEX
		};
	}

	std::array<VkVertexInputAttributeDescription, 3> Vertex::GetInputAttributesDescriptions() ZINET_API_POST
	{
		std::array<VkVertexInputAttributeDescription, 3> descriptions;

		// Position
		descriptions[0] =
		{
			.location = 0,
			.binding = 0,
			.format = VK_FORMAT_R32G32B32_SFLOAT,
			.offset = offsetof(Vertex, position)
		};

		// Color
		descriptions[1] =
		{
			.location = 1,
			.binding = 0,
			.format = VK_FORMAT_R32G32B32_SFLOAT,
			.offset = offsetof(Vertex, color)
		};

		// UV
		descriptions[2] =
		{
			.location = 2,
			.binding = 0,
			.format = VK_FORMAT_R32G32_SFLOAT,
			.offset = offsetof(Vertex, uv)
		};

		return descriptions;
	}

}