#pragma once

#include "Zinet/VulkanRenderer/ZtVulkanRendererConfig.hpp"
#include "Zinet/VulkanRenderer/ZtDescriptorInfo.hpp"

#include <array>

namespace zt::vulkan_renderer
{
	class RendererContext;

	using ShaderModules = std::map<ShaderType, const ShaderModule*>;
	using DescriptorInfos = std::array<DescriptorInfo, 2>;

	struct GraphicsPipelineCreateInfo
	{
		const RendererContext& rendererContext;
		ShaderModules shaderModules;
		DescriptorInfos descriptorInfos;

		size_t descriptorSetsCount = 1;

		bool createVertexInput = true;
	};
}