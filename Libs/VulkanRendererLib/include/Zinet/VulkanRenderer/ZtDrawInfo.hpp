#pragma once

#include "Zinet/VulkanRenderer/ZtVulkanRendererConfig.hpp"
#include "Zinet/VulkanRenderer/ZtVertex.hpp"
#include "Zinet/VulkanRenderer/ZtShaderType.hpp"
#include "Zinet/VulkanRenderer/ZtDescriptorSetLayout.hpp"
#include "Zinet/VulkanRenderer/ZtDescriptorInfo.hpp"

#include "Zinet/Core/ZtFunction.hpp"

#include <span>
#include <map>

namespace zt::vulkan_renderer
{
	class CommandBuffer;
	class DescriptorSets;
	class Buffer;
	class ImageView;
	class ShaderModule;

	struct DrawInfo
	{
		using Vertices = std::vector<Vertex>;
		using Indices = std::vector<std::uint32_t>;

		Buffer* vertexBuffer{};
		Buffer* indexBuffer{};
		uint32_t indexCount{};
		uint32_t instances = 1;
	};
}