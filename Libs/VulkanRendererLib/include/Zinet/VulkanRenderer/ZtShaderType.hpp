#pragma once

#include "Zinet/VulkanRenderer/ZtVulkanRendererConfig.hpp"

#include <glslang/Public/ShaderLang.h>

namespace zt::vulkan_renderer
{
	enum class ShaderType
	{
		Vertex = EShLanguage::EShLangVertex,
		Fragment = EShLanguage::EShLangFragment,
		Compute = EShLanguage::EShLangCompute,
		Invalid
	};

	EShLanguage ShaderTypeToESgLanguage(const ShaderType shaderType) noexcept;

	VkShaderStageFlagBits ShaderTypeToVkShaderStage(const ShaderType shaderType) noexcept;

	ShaderType FromStringToShaderType(const std::string& str);

}