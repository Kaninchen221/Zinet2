#pragma once

#include "Zinet/VulkanRenderer/ZtVulkanRendererConfig.hpp"
#include "Zinet/VulkanRenderer/ZtVulkanObject.hpp"
#include "Zinet/VulkanRenderer/ZtShaderType.hpp"

#include "Zinet/Core/ZtLogger.hpp"

#include <filesystem>

namespace zt::vulkan_renderer
{
	class Device;

	class  ShadersCompiler
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("VRShadersCompiler");

	public:

		using CompileResult = std::vector<uint32_t>;

		ShadersCompiler() noexcept = default;
		ShadersCompiler(const ShadersCompiler& other) noexcept = default;
		ShadersCompiler(ShadersCompiler&& other) noexcept = default;
		~ShadersCompiler() noexcept = default;

		ShadersCompiler& operator = (const ShadersCompiler& other) noexcept = default;
		ShadersCompiler& operator = (ShadersCompiler&& other) noexcept = default;

		CompileResult compileFromFile(const std::filesystem::path& filePath, ShaderType shaderType) const;

		CompileResult compileFromString(const std::string& source, ShaderType shaderType, const std::string& fileName) const;

	};
}