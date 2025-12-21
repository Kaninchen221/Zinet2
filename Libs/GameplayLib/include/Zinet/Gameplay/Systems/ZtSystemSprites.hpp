#pragma once

#include "Zinet/Gameplay/ZtGameplayConfig.hpp"
#include "Zinet/Gameplay/ZtCameraManager.hpp"

#include "Zinet/Gameplay/Assets/ZtAssetTexture.hpp"
#include "Zinet/Gameplay/Assets/ZtAssetSampler.hpp"
#include "Zinet/Gameplay/Assets/ZtAssetShader.hpp"

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

	// TODO: Refactor it
	// I created it because we can't have a two components in the same type in one query
	struct ZINET_GAMEPLAY_API ShaderAssetsPack
	{
		core::ConstAssetHandle<asset::Shader> vertexShaderAsset;
		core::ConstAssetHandle<asset::Shader> fragmentShaderAsset;
	};

	struct ZINET_GAMEPLAY_API SpritesBuffers
	{
		vulkan_renderer::Buffer transform{ nullptr };
		vulkan_renderer::Buffer camera{ nullptr };
	};

	class ZINET_GAMEPLAY_API Sprites
	{
		inline static auto Logger = core::ConsoleLogger::Create("zt::gameplay::system::Sprites");

	public:

		using SpriteQuery = core::ecs::ConstQuery<
			Sprite,
			vulkan_renderer::Transform
			// Texture region
		>;

		using SystemComponentsQuery = core::ecs::Query <
			Sprites,
			vulkan_renderer::GraphicsPipeline,
			ShaderAssetsPack,
			core::ConstAssetHandle<asset::Texture>, // Atlas texture
			core::ConstAssetHandle<asset::Sampler>,
			SpritesBuffers
		>;

		static void Init();

		static void Update(
			core::ecs::WorldCommands worldCommands,
			SpriteQuery sprites,
			SystemComponentsQuery systemComponents,
			core::ecs::ConstResource<vulkan_renderer::VulkanRenderer> rendererRes,
			core::ecs::ConstResource<CameraManager> cameraManagerRes,
			core::ecs::Resource<vulkan_renderer::ResourceStorage> resourceStorageRes);

		static void Deinit(
			core::ecs::WorldCommands worldCommands,
			core::ecs::Query<Sprite, vulkan_renderer::Buffer> query,
			core::ecs::ConstResource<vulkan_renderer::VulkanRenderer> rendererRes
		);

	private:

		static bool CreateTransformBuffer(
			core::ecs::ConstResource<vulkan_renderer::VulkanRenderer> rendererRes,
			SpriteQuery& sprites,
			vulkan_renderer::Buffer& buffer
		);

	};
}