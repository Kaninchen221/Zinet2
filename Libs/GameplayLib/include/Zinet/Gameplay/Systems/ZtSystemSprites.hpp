#pragma once

#include "Zinet/Gameplay/ZtGameplayConfig.hpp"

#include "Zinet/Gameplay/Assets/ZtAssetTexture.hpp"
#include "Zinet/Gameplay/Assets/ZtAssetSampler.hpp"

#include "Zinet/Core/ZtLogger.hpp"
#include "Zinet/Core/ZtExitReason.hpp"
#include "Zinet/Core/ZtPaths.hpp"

#include "Zinet/Core/ECS/ZtWorldCommands.hpp"
#include "Zinet/Core/ECS/ZtResource.hpp"
#include "Zinet/Core/ECS/ZtQuery.hpp"

#include "Zinet/Core/Assets/ZtAssetStorage.hpp"

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
	class ResourceStorage;
}

namespace zt::gameplay::system
{
	struct Sprite
	{};

	class ZINET_GAMEPLAY_API Sprites
	{
		inline static auto Logger = core::ConsoleLogger::Create("zt::gameplay::system::Sprites");

	public:

		struct Data
		{
			vulkan_renderer::Buffer transformBuffer{ nullptr };
			core::ConstAssetHandle<asset::Texture> texture;
			core::ConstAssetHandle<asset::Sampler> sampler;
		};

		static void Init(
			core::ecs::WorldCommands worldCommands,
			core::ecs::ConstQuery<Sprite, vulkan_renderer::Transform> sprites,
			core::ecs::ConstResource<vulkan_renderer::VulkanRenderer> rendererRes,
			core::ecs::ConstResource<core::AssetStorage> assetStorageRes,
			core::ecs::Resource<vulkan_renderer::ResourceStorage> resourceStorageRes);

		static void Update(
			core::ecs::WorldCommands worldCommands,
			core::ecs::ConstQuery<Data> spriteSystemData,
			core::ecs::Resource<vulkan_renderer::ResourceStorage> resourceStorageRes);

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
			const core::AssetStorage& assetStorage,
			std::string assetKey);

		// TODO: Move this function out from the system Sprites scope
		template<class AssetT>
		static core::ConstAssetHandle<AssetT> GetAsset(
			core::ecs::WorldCommands worldCommands,
			const core::AssetStorage& assetStorage,
			std::string assetKey);

	};

	template<class AssetT>
	static core::ConstAssetHandle<AssetT>
		Sprites::GetAsset(
			core::ecs::WorldCommands worldCommands, 
			const core::AssetStorage& assetStorage, 
			std::string assetKey)
	{
		using ResulT = core::ConstAssetHandle<AssetT>;

		auto asset = assetStorage.getAs<AssetT>(assetKey);
		if (!asset)
		{
			worldCommands.addResource(core::ExitReason{ fmt::format("Couldn't get asset: {}", assetKey) });
			return ResulT{ nullptr };
		}

		if (!asset->isLoaded())
		{
			if (!asset->load(core::Paths::RootPath()))
			{
				worldCommands.addResource(core::ExitReason{ "Couldn't load asset" });
				return ResulT{ nullptr };
			}
		}

		return asset;
	}

}