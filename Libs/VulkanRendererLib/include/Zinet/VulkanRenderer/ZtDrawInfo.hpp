#pragma once

#include "Zinet/VulkanRenderer/ZtVulkanRendererConfig.hpp"
#include "Zinet/VulkanRenderer/ZtVertex.hpp"
#include "Zinet/VulkanRenderer/ZtShaderType.hpp"
#include "Zinet/VulkanRenderer/ZtBuffer.hpp"
#include "Zinet/VulkanRenderer/ZtShaderModule.hpp"
#include "Zinet/VulkanRenderer/ZtImageView.hpp"
#include "Zinet/VulkanRenderer/ZtSampler.hpp"

#include <span>

namespace zt::vulkan_renderer
{
	class Texture;
	class Sampler;
	class CommandBuffer;

	struct ZINET_VULKAN_RENDERER_API TextureInfo
	{
		Texture* texture{};
		Sampler* sampler{};
		ShaderType shaderType = ShaderType::Invalid;
	};

	struct ZINET_VULKAN_RENDERER_API UniformBufferInfo
	{
		Buffer* uniformBuffer{};
	};

	struct ZINET_VULKAN_RENDERER_API DescriptorInfo
	{
		/// Uniform buffer per instance
		std::vector<UniformBufferInfo> uniformBuffers;

		/// Texture per instance
		std::vector<TextureInfo> texturesInfos;

		/// Cached data
		uint32_t cachedUniformBuffersBinding;
		uint32_t cachedTexturesBinding;
	};

	struct ZINET_VULKAN_RENDERER_API DrawInfo
	{
		using Vertices = std::vector<Vertex>;
		using Indices = std::vector<std::uint16_t>;

		ShaderModule* vertexShaderModule{};
		ShaderModule* fragmentShaderModule{};
		Buffer* vertexBuffer{};
		Buffer* indexBuffer{};
		uint32_t indexCount{};
		uint32_t instances = 1;

		DescriptorInfo pipelineDescriptorInfo;
		DescriptorInfo drawCallDescriptorInfo;

		using AdditionalCommands = std::vector<void(*)(const CommandBuffer&)>;
		AdditionalCommands additionalCommands;
	};
}