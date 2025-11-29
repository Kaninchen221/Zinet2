#pragma once

#include "Zinet/Gameplay/ZtGameplayConfig.hpp"

#include "Zinet/Core/ZtLogger.hpp"

#include "Zinet/Core/ECS/ZtWorldCommands.hpp"
#include "Zinet/Core/ECS/ZtResource.hpp"

#include "Zinet/Core/Assets/ZtAssetsStorage.hpp"

#include "Zinet/Window/ZtWindow.hpp"
#include "Zinet/Window/ZtWindowEvents.hpp"

#include "Zinet/VulkanRenderer/ZtVulkanRenderer.hpp"
#include "Zinet/VulkanRenderer/ZtShaderModule.hpp"

#include <string>

namespace zt::vulkan_renderer
{
	class VulkanRenderer;
}

namespace zt::gameplay::system
{
	class ZINET_GAMEPLAY_API Sprites
	{
		inline static auto Logger = core::ConsoleLogger::Create("zt::gameplay::system::Sprites");

	public:

		static void Init(
			core::ecs::WorldCommands worldCommands, 
			core::ecs::ConstResource<vulkan_renderer::VulkanRenderer> rendererRes,
			core::ecs::ConstResource<core::AssetsStorage> assetsStorageRes);

		static void Update(core::ecs::WorldCommands) {}

		static void Deinit(core::ecs::WorldCommands) {}

	private:

		static vulkan_renderer::ShaderModule CreateShaderModule(
			core::ecs::WorldCommands worldCommands,
			const vulkan_renderer::VulkanRenderer& renderer,
			const core::AssetsStorage& assetsStorage,
			std::string assetShaderKey);

// 		static vulkan_renderer::DescriptorInfo CreateDescriptorInfo(
// 
// 		);

	};
}