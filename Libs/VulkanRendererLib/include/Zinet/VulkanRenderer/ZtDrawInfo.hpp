#pragma once

#include "Zinet/VulkanRenderer/ZtVulkanRendererConfig.hpp"

namespace zt::vulkan_renderer
{
	class ShaderModule;

	class DrawInfo 
	{
	public:

		const ShaderModule& vertexShaderModule;
		const ShaderModule& fragmentShaderModule;
	};
}