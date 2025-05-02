#include "Zinet/VulkanRenderer/ZtVertex.hpp"

namespace zt::vulkan_renderer
{

	VkVertexInputBindingDescription Vertex::GetInputBindingDescription() noexcept
	{
		VkVertexInputBindingDescription bindingDescription{};
		bindingDescription.binding = 0;
		bindingDescription.stride = sizeof(Vertex);
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		return bindingDescription;
	}

	std::array<VkVertexInputAttributeDescription, 2> Vertex::GetInputAttributesDescriptions() noexcept
	{
		std::array<VkVertexInputAttributeDescription, 2> descriptions;

		// Position
		descriptions[0].binding = 0;
		descriptions[0].location = 0;
		descriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
		descriptions[0].offset = offsetof(Vertex, position);

		// Color
		descriptions[1].binding = 0;
		descriptions[1].location = 1;
		descriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		descriptions[1].offset = offsetof(Vertex, color);

		return descriptions;
	}

}