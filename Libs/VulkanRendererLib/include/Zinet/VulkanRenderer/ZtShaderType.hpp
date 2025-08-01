#pragma once

#include "Zinet/VulkanRenderer/ZtVulkanRendererConfig.hpp"

#include <shaderc/shaderc.hpp>

namespace zt::vulkan_renderer
{
	enum class ShaderType
	{
		Vertex = shaderc_shader_kind::shaderc_vertex_shader,
		Fragment = shaderc_shader_kind::shaderc_fragment_shader,
		Compute = shaderc_shader_kind::shaderc_compute_shader,
		Invalid
	};

	ZINET_VULKAN_RENDERER_API shaderc_shader_kind ShaderTypeToShaderCShaderKind(const ShaderType shaderType) ZINET_API_POST;

	ZINET_VULKAN_RENDERER_API VkShaderStageFlagBits ShaderTypeToVkShaderStage(const ShaderType shaderType) ZINET_API_POST;

	ZINET_VULKAN_RENDERER_API ShaderType FromStringToShaderType(const std::string& str) ZINET_API_POST;

}