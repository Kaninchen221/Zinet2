#include "Zinet/VulkanRenderer/ZtShaderType.hpp"

#include "Zinet/Core/ZtLogger.hpp"

namespace zt::vulkan_renderer
{
	shaderc_shader_kind ShaderTypeToShaderCShaderKind(const ShaderType shaderType) noexcept
	{
		switch (shaderType)
		{
		case ShaderType::Compute: return shaderc_shader_kind::shaderc_compute_shader;
		case ShaderType::Fragment: return shaderc_shader_kind::shaderc_glsl_fragment_shader;
		case ShaderType::Vertex: return shaderc_shader_kind::shaderc_glsl_vertex_shader;
		}

		Ensure(false); // Not supported ShaderType
		return shaderc_shader_kind::shaderc_glsl_infer_from_source;
	}

	VkShaderStageFlagBits ShaderTypeToVkShaderStage(const ShaderType shaderType) noexcept
	{
		switch (shaderType)
		{
		case ShaderType::Compute: return VK_SHADER_STAGE_COMPUTE_BIT;
		case ShaderType::Fragment: return VK_SHADER_STAGE_FRAGMENT_BIT;
		case ShaderType::Vertex: return VK_SHADER_STAGE_VERTEX_BIT;
		}

		Ensure(false); // Not supported ShaderType
		return {};
	}

	ShaderType FromStringToShaderType(const std::string& str)
	{
		if (str == "vert")
			return ShaderType::Vertex;
		else if (str == "frag")
			return ShaderType::Fragment;
		else if (str == "comp")
			return ShaderType::Compute;

		Ensure(false); // Not supported ShaderType
		return ShaderType::Invalid;
	}
}