#pragma once

#include "Zinet/VulkanRenderer/ZtVulkanRendererConfig.hpp"
#include "Zinet/VulkanRenderer/ZtVertex.hpp"
#include "Zinet/VulkanRenderer/ZtShaderType.hpp"

#include <span>

namespace zt::vulkan_renderer
{
	class ShaderModule;
	class Buffer;
	class Texture;
	class Sampler;

	struct ZINET_VULKAN_RENDERER_API TextureInfo
	{
		Texture* texture{};
		Sampler* sampler{};
		ShaderType shaderType = ShaderType::Invalid;
	};

	class ZINET_VULKAN_RENDERER_API DrawInfo
	{
	public:

		using Vertices = std::vector<Vertex>;
		using Indices = std::vector<std::uint16_t>;

		const ShaderModule& vertexShaderModule;
		const ShaderModule& fragmentShaderModule;
		const Buffer& vertexBuffer;
		const Buffer& indexBuffer;
		const std::uint32_t indexCount{};
		const Buffer& uniformBuffer;
		std::vector<TextureInfo> textureInfos;
	};
}