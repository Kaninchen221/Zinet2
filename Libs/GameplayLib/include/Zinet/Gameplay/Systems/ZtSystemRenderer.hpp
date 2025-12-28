#pragma once

#include "Zinet/Gameplay/ZtGameplayConfig.hpp"

#include "Zinet/Gameplay/ZtRenderCommand.hpp"

#include "Zinet/Core/ZtLogger.hpp"

#include "Zinet/Core/ECS/ZtWorldCommands.hpp"
#include "Zinet/Core/ECS/ZtResource.hpp"
#include "Zinet/Core/ECS/ZtQuery.hpp"

#include "Zinet/VulkanRenderer/ZtResourceStorage.hpp"

namespace zt::wd
{
	class Window;
}

namespace zt::vulkan_renderer
{
	class VulkanRenderer;
}

namespace zt::gameplay::system
{
	class ZINET_GAMEPLAY_API Renderer
	{
		inline static auto Logger = core::ConsoleLogger::Create("zt::gameplay::system::Renderer");
	
	public:

		using DrawDataQuery = core::ecs::ConstQuery<
			vulkan_renderer::GraphicsPipeline, vulkan_renderer::DrawInfo>;

		static void Init(core::ecs::WorldCommands worldCommands, core::ecs::Resource<wd::Window> windowRes);

		static void Update(
			core::ecs::WorldCommands worldCommands, 
			core::ecs::ConstResource<wd::Window> windowRes,
			core::ecs::Resource<vulkan_renderer::VulkanRenderer> rendererRes,
			core::ecs::ConstQuery<RenderCommand> drawCommandQuery,
			DrawDataQuery drawDataQuery,
			core::ecs::Resource<vulkan_renderer::ResourceStorage> resourceStorageRes);

		static void Deinit(
			core::ecs::Resource<vulkan_renderer::VulkanRenderer> rendererRes,
			core::ecs::Resource<vulkan_renderer::ResourceStorage> resourceStorageRes);
	};

}