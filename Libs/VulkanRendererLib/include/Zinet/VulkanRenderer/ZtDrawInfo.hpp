#pragma once

#include "Zinet/VulkanRenderer/ZtVulkanRendererConfig.hpp"
#include "Zinet/VulkanRenderer/ZtVertex.hpp"

namespace zt::vulkan_renderer
{
	class ShaderModule;
	class Buffer;

	class ZINET_VULKAN_RENDERER_API DrawInfo
	{
	public:

		using Vertices = std::vector<Vertex>;

		const ShaderModule& vertexShaderModule;
		const ShaderModule& fragmentShaderModule;
		const Buffer& vertexBuffer;

	};
}