#pragma once

#include "Zinet/VulkanRenderer/ZtVulkanRendererConfig.hpp"
#include "Zinet/VulkanRenderer/ZtShaderType.hpp"

namespace zt::vulkan_renderer
{
	class Texture;
	class Sampler;

	struct  TextureInfo
	{
		const Texture* texture{};
		const Sampler* sampler{};
		ShaderType shaderType = ShaderType::Invalid;
	};
}