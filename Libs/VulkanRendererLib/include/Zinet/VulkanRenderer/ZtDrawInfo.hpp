#pragma once

#include "Zinet/VulkanRenderer/ZtVulkanRendererConfig.hpp"
#include "Zinet/VulkanRenderer/ZtVertex.hpp"
#include "Zinet/VulkanRenderer/ZtShaderType.hpp"
#include "Zinet/VulkanRenderer/ZtBuffer.hpp"
#include "Zinet/VulkanRenderer/ZtShaderModule.hpp"

#include <span>

namespace zt::vulkan_renderer
{
	class Texture;
	class Sampler;

	struct ZINET_VULKAN_RENDERER_API TextureInfo
	{
		Texture* texture{};
		Sampler* sampler{};
		ShaderType shaderType = ShaderType::Invalid;
	};

	struct ZINET_VULKAN_RENDERER_API DescriptorInfo
	{
		Buffer* uniformBuffer{};
		std::vector<TextureInfo> texturesInfos;
	};

	struct ZINET_VULKAN_RENDERER_API DrawInfo
	{
		using Vertices = std::vector<Vertex>;
		using Indices = std::vector<std::uint16_t>;

		ShaderModule* vertexShaderModule{};
		ShaderModule* fragmentShaderModule{};
		Buffer* vertexBuffer{};
		Buffer* indexBuffer{};
		std::uint32_t indexCount{};

		DescriptorInfo pipelineDescriptorInfo;
		DescriptorInfo objectDescriptorInfo;
	};
}