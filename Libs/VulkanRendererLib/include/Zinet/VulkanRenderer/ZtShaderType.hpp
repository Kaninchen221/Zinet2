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

	 shaderc_shader_kind ShaderTypeToShaderCShaderKind(const ShaderType shaderType) noexcept;

	 VkShaderStageFlagBits ShaderTypeToVkShaderStage(const ShaderType shaderType) noexcept;

	 ShaderType FromStringToShaderType(const std::string& str);

}