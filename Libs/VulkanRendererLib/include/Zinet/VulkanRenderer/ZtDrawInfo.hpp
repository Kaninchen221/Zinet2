#pragma once

#include "Zinet/VulkanRenderer/ZtVulkanRendererConfig.hpp"
#include "Zinet/VulkanRenderer/ZtVertex.hpp"
#include "Zinet/VulkanRenderer/ZtShaderType.hpp"
#include "Zinet/VulkanRenderer/ZtBuffer.hpp"
#include "Zinet/VulkanRenderer/ZtShaderModule.hpp"
#include "Zinet/VulkanRenderer/ZtImageView.hpp"
#include "Zinet/VulkanRenderer/ZtSampler.hpp"

#include "Zinet/Core/ZtFunction.hpp"

#include <span>

namespace zt::vulkan_renderer
{
	class Texture;
	class Sampler;
	class CommandBuffer;

	struct  TextureInfo
	{
		Texture* texture{};
		Sampler* sampler{};
		ShaderType shaderType = ShaderType::Invalid;
	};

	struct  UniformBufferInfo
	{
		Buffer* uniformBuffer{};
	};

	struct DescriptorInfo
	{
		/// Uniform buffer per instance
		std::vector<UniformBufferInfo> uniformBuffers;

		/// Texture per instance
		std::vector<TextureInfo> texturesInfos;

		DescriptorInfo& operator += (const DescriptorInfo& other)
		{
			uniformBuffers.append_range(other.uniformBuffers);
			texturesInfos.append_range(other.texturesInfos);
			return *this;
		}

		/// Cached data
		uint32_t cachedUniformBuffersBinding{};
		uint32_t cachedTexturesBinding{};
	};

	struct DrawInfo
	{
		using Vertices = std::vector<Vertex>;
		using Indices = std::vector<std::uint16_t>;

		ShaderModule* vertexShaderModule{};
		ShaderModule* fragmentShaderModule{};
		Buffer* vertexBuffer{};
		Buffer* indexBuffer{};
		uint32_t indexCount{};
		uint32_t instances = 1;

		bool updatePipelineDescriptorInfoPerDrawCall = true;
		DescriptorInfo pipelineDescriptorInfo;
		DescriptorInfo drawCallDescriptorInfo;

		using AdditionalCommands = std::vector<core::Function<void, const CommandBuffer&>>;
		AdditionalCommands additionalCommands;
	};
}