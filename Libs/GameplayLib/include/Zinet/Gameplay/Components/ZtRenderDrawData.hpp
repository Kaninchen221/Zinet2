#pragma once

#include "Zinet/Gameplay/ZtGameplayConfig.hpp"

#include "Zinet/VulkanRenderer/ZtVulkanRenderer.hpp"

namespace zt::gameplay::component
{
	struct ZINET_GAMEPLAY_API RenderDrawData
	{
		using Command = vulkan_renderer::VulkanRenderer::Command;

		Command command;
	};
}