#pragma once

#include "Zinet/VulkanRenderer/ZtVulkanRendererConfig.hpp"
#include "Zinet/VulkanRenderer/ZtVulkanObject.hpp"
#include "Zinet/VulkanRenderer/ZtShadersCompiler.hpp"

#include "Zinet/Core/ZtLogger.hpp"

#include <vulkan/vulkan.h>

#include <shaderc/shaderc.hpp>

namespace zt::vulkan_renderer
{
	class Device;

	class ZINET_VULKAN_RENDERER_API ShaderModule : public VulkanObject<VkShaderModule>
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("VRShaderModule");

	public:

		ShaderModule(HandleType newObjectHandle)
			: VulkanObject(newObjectHandle)
		{}

		ShaderModule() ZINET_API_POST = delete;
		ShaderModule(const ShaderModule& other) ZINET_API_POST = delete;
		ShaderModule(ShaderModule&& other) ZINET_API_POST = default;
		~ShaderModule() ZINET_API_POST = default;

		ShaderModule& operator = (const ShaderModule& other) ZINET_API_POST = delete;
		ShaderModule& operator = (ShaderModule&& other) ZINET_API_POST = default;

		bool create(const Device& device, const shaderc::SpvCompilationResult& compilationResult) ZINET_API_POST;

		VkPipelineShaderStageCreateInfo createPipelineShaderStageCreateInfo(const ShaderType shaderType) const ZINET_API_POST;

		void destroy(const Device& device) ZINET_API_POST;

	};
}