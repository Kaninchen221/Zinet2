#pragma once

#include "Zinet/Gameplay/ZtGameplayConfig.hpp"

#include "Zinet/Gameplay/ZtRenderDrawData.hpp"

#include "Zinet/Core/ZtLogger.hpp"

#include "Zinet/Core/ECS/ZtWorldCommands.hpp"
#include "Zinet/Core/ECS/ZtResource.hpp"
#include "Zinet/Core/ECS/ZtQuery.hpp"

namespace zt::wd
{
	class Window;
}

namespace zt::vulkan_renderer
{
	class VulkanRenderer;
	class ImGuiIntegration;
}

namespace zt::gameplay
{
	struct ZINET_GAMEPLAY_API ImGuiData
	{
		bool skipImGui = false;
	};
}

namespace zt::gameplay::system
{
	class ZINET_GAMEPLAY_API ImGui
	{
		inline static auto Logger = core::ConsoleLogger::Create("zt::gameplay::system::ImGui");

	public:

		struct Pre{};
		struct Post{};

		static void Init(
			core::ecs::WorldCommands worldCommands,
			core::ecs::Resource<wd::Window> windowRes,
			core::ecs::ConstResource<vulkan_renderer::VulkanRenderer> rendererRes);

		static void PreUpdate(
			core::ecs::Resource<ImGuiData> imGuiData,
			core::ecs::Query<RenderDrawData, ImGui> imGuiRenderDrawDataQuery,
			core::ecs::ConstResource<wd::Window> windowRes);

		static void PostUpdate(
			core::ecs::WorldCommands worldCommands,
			core::ecs::ConstResource<wd::Window> windowRes,
			core::ecs::ConstResource<vulkan_renderer::ImGuiIntegration> imGuiIntegrationRes);

		static void Deinit(
			core::ecs::WorldCommands worldCommands,
			core::ecs::ConstResource<vulkan_renderer::VulkanRenderer> rendererRes,
			core::ecs::Resource<vulkan_renderer::ImGuiIntegration> imGuiIntegrationRes);
	};
}