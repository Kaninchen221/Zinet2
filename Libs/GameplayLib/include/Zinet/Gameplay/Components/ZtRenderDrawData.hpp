#pragma once

#include "Zinet/Gameplay/ZtGameplayConfig.hpp"

#include "Zinet/VulkanRenderer/ZtVulkanRenderer.hpp"

// TODO: Again... I think storing this kind of classes is a bad habit because they can be used as a resource
namespace zt::gameplay::component
{
	struct ZINET_GAMEPLAY_API RenderDrawData
	{
		using Command = vulkan_renderer::VulkanRenderer::Command;

		bool shouldDraw = true;
		Command command;
	};
}