#pragma once

#include "Zinet/Gameplay/ZtGameplayConfig.hpp"

#include "Zinet/VulkanRenderer/ZtVulkanRenderer.hpp"

namespace zt::gameplay
{
	struct ZINET_GAMEPLAY_API RenderCommand
	{
		using Command = vulkan_renderer::VulkanRenderer::Command;

		bool shouldDraw = true; // TODO: Test it
		Command command;
	};
}