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
		vulkan_renderer::Buffer vertex{ nullptr };
		vulkan_renderer::Buffer index{ nullptr };
		vulkan_renderer::Buffer position{ nullptr };
		vulkan_renderer::Buffer rotation{ nullptr };
		vulkan_renderer::Buffer scale{ nullptr };
		vulkan_renderer::Buffer camera{ nullptr };

		void destroy(const vulkan_renderer::VMA& vma) noexcept
		{
			vertex.destroy(vma);
			index.destroy(vma);
			position.destroy(vma);
			rotation.destroy(vma);
			scale.destroy(vma);
			camera.destroy(vma);
		}
	};

	class ZINET_GAMEPLAY_API Sprites
	{
		inline static auto Logger = core::ConsoleLogger::Create("zt::gameplay::system::Sprites");

	public:

		using SpriteQuery = core::ecs::ConstQuery<
			Sprite,
			Position,
			Rotation,
			Scale
			// Texture region
		>;

		using SystemComponentsQuery = core::ecs::Query<
			Sprites,
			vulkan_renderer::GraphicsPipeline,
			vulkan_renderer::DrawInfo,
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
			SystemComponentsQuery systemComponents,
			core::ecs::ConstResource<vulkan_renderer::VulkanRenderer> rendererRes
		);

	private:

		static bool CreateVertexBuffer(
			core::ecs::ConstResource<vulkan_renderer::VulkanRenderer> rendererRes,
			vulkan_renderer::Buffer& buffer
		);

		static bool CreateIndexBuffer(
			core::ecs::ConstResource<vulkan_renderer::VulkanRenderer> rendererRes,
			vulkan_renderer::Buffer& buffer
		);

		static bool CreateComponentBuffer(
			core::ecs::ConstResource<vulkan_renderer::VulkanRenderer> rendererRes,
			const auto& componentsPack,
			vulkan_renderer::Buffer& buffer
		);

		static bool UpdateComponentBuffer(
			core::ecs::ConstResource<vulkan_renderer::VulkanRenderer> rendererRes,
			const auto& componentsPack,
			vulkan_renderer::Buffer& buffer
		);

		static bool CreateCameraBuffer(
			core::ecs::ConstResource<vulkan_renderer::VulkanRenderer> rendererRes,
			vulkan_renderer::Buffer& buffer
		);

		static bool UpdateCameraBuffer(
			core::ecs::ConstResource<vulkan_renderer::VulkanRenderer> rendererRes,
			core::ecs::ConstResource<CameraManager> cameraManagerRes,
			vulkan_renderer::Buffer& buffer
		);
	};

	bool Sprites::CreateComponentBuffer(core::ecs::ConstResource<vulkan_renderer::VulkanRenderer> rendererRes, const auto& componentsPack, vulkan_renderer::Buffer& buffer)
	{
		using namespace core;
		using namespace vulkan_renderer;

		auto& rendererContext = rendererRes->getRendererContext();
		auto& vma = rendererContext.getVMA();

		if (buffer)
		{
			Logger->warn("Passed a valid buffer");
			return false;
		}

		VkDeviceSize bufferSize{};
		for (auto components : componentsPack)
		{
			if (!components)
			{
				Logger->error("Components is null");
				return false;
			}

			bufferSize += components->getObjectsCapacity();
		}
		if (bufferSize <= 0)
			return false;

		bufferSize *= componentsPack.front()->getTypeSize();

		VkBufferCreateInfo createInfo
		{
			.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
			.size = bufferSize,
			.usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			.sharingMode = VK_SHARING_MODE_EXCLUSIVE
		};

		if (!buffer.create(vma, createInfo))
		{
			Logger->error("Couldn't create a component buffer");
			return false;
		}

		return buffer;
	}

	bool Sprites::UpdateComponentBuffer(core::ecs::ConstResource<vulkan_renderer::VulkanRenderer> rendererRes, const auto& componentsPack, vulkan_renderer::Buffer& buffer)
	{
		using namespace vulkan_renderer;

		const auto& rendererContext = rendererRes->getRendererContext();
		const auto& vma = rendererContext.getVMA();

		if (!buffer)
		{
			Logger->error("Buffer is invalid");
			return false;
		}

		size_t offset = 0;
		for (auto components : componentsPack)
		{
			if (!components)
			{
				Logger->error("Components is null");
				return false;
			}

			const auto data = components->data();
			// TODO: Handle situations when we remove entities (so some components are invalid, perhaps additional buffer for discarding?)
			const size_t bufferSize = components->getObjectsCount() * components->getTypeSize();
			buffer.fillWithData(vma, data, bufferSize, offset);

			offset += bufferSize;
		}

		return true;
	}
}