#include "Zinet/VulkanRenderer/ZtShaderType.hpp"

#include "Zinet/Core/ZtLogger.hpp"
#include "Zinet/Core/ZtDebug.hpp"

namespace zt::vulkan_renderer
{
	EShLanguage ShaderTypeToESgLanguage(const ShaderType shaderType) noexcept
	{
		switch (shaderType)
		{
		case ShaderType::Compute: return EShLanguage::EShLangCompute;
		case ShaderType::Fragment: return EShLanguage::EShLangFragment;
		case ShaderType::Vertex: return EShLanguage::EShLangVertex;
		}

		Ensure(false, "Not supported ShaderType"); 
		return EShLanguage::EShLangVertex;
	}

	VkShaderStageFlagBits ShaderTypeToVkShaderStage(const ShaderType shaderType) noexcept
	{
		switch (shaderType)
		{
		case ShaderType::Compute: return VK_SHADER_STAGE_COMPUTE_BIT;
		case ShaderType::Fragment: return VK_SHADER_STAGE_FRAGMENT_BIT;
		case ShaderType::Vertex: return VK_SHADER_STAGE_VERTEX_BIT;
		}

		Ensure(false, "Not supported ShaderType");
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

		Ensure(false, "Not supported ShaderType");
		return ShaderType::Invalid;
	}
}