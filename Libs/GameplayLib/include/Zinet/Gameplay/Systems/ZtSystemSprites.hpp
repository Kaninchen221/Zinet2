#pragma once

#include "Zinet/Gameplay/ZtGameplayConfig.hpp"

#include "Zinet/Gameplay/Assets/ZtAssetTexture.hpp"

#include "Zinet/Core/ZtLogger.hpp"

#include "Zinet/Core/ECS/ZtWorldCommands.hpp"
#include "Zinet/Core/ECS/ZtResource.hpp"
#include "Zinet/Core/ECS/ZtQuery.hpp"

#include "Zinet/Core/Assets/ZtAssetsStorage.hpp"

#include "Zinet/Window/ZtWindow.hpp"
#include "Zinet/Window/ZtWindowEvents.hpp"

#include "Zinet/VulkanRenderer/ZtVulkanRenderer.hpp"
#include "Zinet/VulkanRenderer/ZtShaderModule.hpp"
#include "Zinet/VulkanRenderer/ZtBuffer.hpp"
#include "Zinet/VulkanRenderer/ZtTransform.hpp"

#include <string>

namespace zt::vulkan_renderer
{
	class VulkanRenderer;
}

namespace zt::gameplay::system
{
	struct Sprite
	{};

	class ZINET_GAMEPLAY_API Sprites
	{
		inline static auto Logger = core::ConsoleLogger::Create("zt::gameplay::system::Sprites");

	public:

		static void Init(
			core::ecs::WorldCommands worldCommands,
			core::ecs::ConstQuery<Sprite, vulkan_renderer::Transform> sprites,
			core::ecs::ConstResource<vulkan_renderer::VulkanRenderer> rendererRes,
			core::ecs::ConstResource<core::AssetsStorage> assetsStorageRes);

		static void Update(core::ecs::WorldCommands) {}

		static void Deinit(
			core::ecs::WorldCommands worldCommands,
			core::ecs::Query<Sprite, vulkan_renderer::Buffer> query,
			core::ecs::ConstResource<vulkan_renderer::VulkanRenderer> rendererRes
		);

	private:

		static vulkan_renderer::Buffer CreateTransformBuffer(
			core::ecs::ConstResource<vulkan_renderer::VulkanRenderer> rendererRes,
			core::ecs::ConstQuery<Sprite, vulkan_renderer::Transform>& sprites
		);

		static vulkan_renderer::ShaderModule CreateShaderModule(
			core::ecs::WorldCommands worldCommands,
			const vulkan_renderer::VulkanRenderer& renderer,
			const core::AssetsStorage& assetsStorage,
			std::string assetShaderKey);

		static core::ConstAssetHandle<asset::Texture> GetTexture(
			core::ecs::WorldCommands worldCommands,
			const core::AssetsStorage& assetsStorage,
			std::string assetShaderKey);

	};
}