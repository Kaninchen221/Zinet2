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

		ShadersCompiler() noexcept = default;
		ShadersCompiler(const ShadersCompiler& other) noexcept = default;
		ShadersCompiler(ShadersCompiler&& other) noexcept = default;
		~ShadersCompiler() noexcept = default;

		ShadersCompiler& operator = (const ShadersCompiler& other) noexcept = default;
		ShadersCompiler& operator = (ShadersCompiler&& other) noexcept = default;

		shaderc::SpvCompilationResult compileFromFile(const std::filesystem::path& filePath, const ShaderType shaderType) const noexcept;

	};
}