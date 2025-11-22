#pragma once

#include "Zinet/Gameplay/ZtGameplayConfig.hpp"

#include "Zinet/Core/ZtLogger.hpp"

#include "Zinet/Core/ECS/ZtWorldCommands.hpp"
#include "Zinet/Core/ECS/ZtResource.hpp"

namespace zt::wd
{
	class Window;
}

namespace zt::vulkan_renderer
{
	class VulkanRenderer;
	class ImGuiIntegration;
}

namespace zt::gameplay::system
{
	class ZINET_GAMEPLAY_API ImGui
	{
		inline static auto Logger = core::ConsoleLogger::Create("zt::gameplay::system::ImGui");

	public:

		static void Init(
			core::ecs::WorldCommands worldCommands,
			core::ecs::Resource<wd::Window> windowRes,
			core::ecs::ConstResource<vulkan_renderer::VulkanRenderer> rendererRes);

		static void Update(
			core::ecs::WorldCommands worldCommands,
			core::ecs::ConstResource<vulkan_renderer::ImGuiIntegration> imGuiIntegrationRes);

		static void Deinit(
			core::ecs::WorldCommands worldCommands,
			core::ecs::ConstResource<vulkan_renderer::VulkanRenderer> rendererRes,
			core::ecs::Resource<vulkan_renderer::ImGuiIntegration> imGuiIntegrationRes);
	};
}