#pragma once

#include "Zinet/VulkanRenderer/ZtVulkanRendererConfig.hpp"
#include "Zinet/VulkanRenderer/ZtVulkanObject.hpp"
#include "Zinet/VulkanRenderer/ZtShadersCompiler.hpp"

#include "Zinet/Core/ZtLogger.hpp"

#include <vulkan/vulkan.h>

namespace zt::vulkan_renderer
{
	class Device;
	class RendererContext;

	class ShaderModule : public VulkanObject<VkShaderModule>
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("VRShaderModule");

	public:

		ShaderModule(HandleType newObjectHandle) noexcept
			: VulkanObject(newObjectHandle)
		{}

		ShaderModule() noexcept = delete;
		ShaderModule(const ShaderModule& other) noexcept = delete;
		ShaderModule(ShaderModule&& other) noexcept = default;
		~ShaderModule() noexcept = default;

		ShaderModule& operator = (const ShaderModule& other) noexcept = delete;
		ShaderModule& operator = (ShaderModule&& other) noexcept = default;

		bool create(const Device& device, const ShadersCompiler::CompileResult& compilationResult);

		VkPipelineShaderStageCreateInfo createPipelineShaderStageCreateInfo(const ShaderType shaderType) const;

		void destroy(const Device& device) noexcept;

		void destroy(const RendererContext& rendererContext) noexcept;

	};
}