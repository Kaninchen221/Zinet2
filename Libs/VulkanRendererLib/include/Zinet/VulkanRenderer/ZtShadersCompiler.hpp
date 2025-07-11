#pragma once

#include "Zinet/VulkanRenderer/ZtVulkanRendererConfig.hpp"
#include "Zinet/VulkanRenderer/ZtVulkanObject.hpp"
#include "Zinet/VulkanRenderer/ZtShaderType.hpp"

#include "Zinet/Core/ZtLogger.hpp"

#include <shaderc/shaderc.hpp>

#include <filesystem>

namespace zt::vulkan_renderer
{
	class Device;

	class ZINET_VULKAN_RENDERER_API ShadersCompiler
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("VRShadersCompiler");

	public:

		ShadersCompiler() ZINET_API_POST = default;
		ShadersCompiler(const ShadersCompiler& other) ZINET_API_POST = default;
		ShadersCompiler(ShadersCompiler&& other) ZINET_API_POST = default;
		~ShadersCompiler() ZINET_API_POST = default;

		ShadersCompiler& operator = (const ShadersCompiler& other) ZINET_API_POST = default;
		ShadersCompiler& operator = (ShadersCompiler&& other) ZINET_API_POST = default;

		shaderc::SpvCompilationResult compileFromFile(const std::filesystem::path& filePath, const ShaderType shaderType) const ZINET_API_POST;

	};
}