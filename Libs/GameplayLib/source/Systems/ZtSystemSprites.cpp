#include "Zinet/Gameplay/Systems/ZtSystemSprites.hpp"

#include "Zinet/Gameplay/Assets/ZtAssetShader.hpp"
#include "Zinet/Gameplay/Assets/ZtAssetTexture.hpp"

#include "Zinet/VulkanRenderer/ZtGraphicsPipeline.hpp"
#include "Zinet/VulkanRenderer/ZtVulkanRenderer.hpp"
#include "Zinet/VulkanRenderer/ZtShaderModule.hpp"

namespace zt::gameplay
{
	namespace system
	{
		using namespace core;
		using namespace vulkan_renderer;

		void Sprites::Init(
			ecs::WorldCommands worldCommands,
			ecs::ConstQuery<Sprite, vulkan_renderer::Transform> sprites,
			ecs::ConstResource<VulkanRenderer> rendererRes,
			ecs::ConstResource<core::AssetStorage> assetStorageRes)
		{
			// TODO: Refactor to validate method
			if (!rendererRes)
			{
				worldCommands.addResource(ExitReason{ "Expected rendererRes" });
				return;
			}

			if (!rendererRes->isInitialized())
			{
				worldCommands.addResource(ExitReason{ "Expected initialized renderer" });
				return;
			}

			if (!assetStorageRes)
			{
				worldCommands.addResource(ExitReason{ "Expected assetStorageRes" });
				return;
			}

			auto transformBuffer = CreateTransformBuffer(rendererRes, sprites);

			std::map<ShaderType, const ShaderModule*> shaderModules;

			// Shader Modules
			auto vertexShaderModule = CreateShaderModule(worldCommands, *rendererRes, *assetStorageRes, "Content/Shaders/shader_sprites.vert");
			if (!vertexShaderModule)
				return;
			
			shaderModules.insert({ ShaderType::Vertex, &vertexShaderModule });

			auto fragmentShaderModule = CreateShaderModule(worldCommands, *rendererRes, *assetStorageRes, "Content/Shaders/shader_sprites.frag");
			if (!fragmentShaderModule)
				return;
			
			shaderModules.insert({ ShaderType::Fragment, &fragmentShaderModule });

			// Texture
			auto texture = GetAsset<asset::Texture>(worldCommands, *assetStorageRes, "Content/Textures/default_texture.png");
			if (!texture)
				return;

			// Sampler
			auto sampler = GetAsset<asset::Sampler>(worldCommands, *assetStorageRes, "Content/Samplers/linear.sampler");
			if (!sampler)
				return;

			worldCommands.spawn(Sprite{}, std::move(transformBuffer), texture, sampler);

			// Destroy shader modules
			{
				auto& device = rendererRes->getRendererContext().getDevice();
				vertexShaderModule.destroy(device);
				fragmentShaderModule.destroy(device);
			}
		}

		void Sprites::Deinit(
			core::ecs::WorldCommands worldCommands,
			core::ecs::Query<Sprite, vulkan_renderer::Buffer> query,
			core::ecs::ConstResource<vulkan_renderer::VulkanRenderer> rendererRes
		)
		{
			if (!rendererRes)
			{
				worldCommands.addResource(ExitReason{ "Expected rendererRes" });
				return;
			}

			auto& rendererContext = rendererRes->getRendererContext();
			auto& vma = rendererContext.getVMA();

			for (auto [sprites, buffer] : query)
			{
				buffer->destroy(vma);
			}
		}

		ShaderModule Sprites::CreateShaderModule(
			ecs::WorldCommands worldCommands,
			const VulkanRenderer& renderer,
			const AssetStorage& assetStorage,
			std::string assetKey)
		{
			ShaderModule shaderModule{ nullptr };

			auto shader = GetAsset<asset::Shader>(worldCommands, assetStorage, assetKey);
			if (!shader)
				return shaderModule;

			if (!shaderModule.create(renderer.getRendererContext().getDevice(), shader->getCompileResult()))
			{
				worldCommands.addResource(ExitReason{ "Couldn't create a shader module" });
				return { nullptr };
			}

			return shaderModule;
		}

		Buffer Sprites::CreateTransformBuffer(
			ecs::ConstResource<vulkan_renderer::VulkanRenderer> rendererRes, 
			ecs::ConstQuery<Sprite, vulkan_renderer::Transform>& sprites)
		{
			using namespace core;
			using namespace vulkan_renderer;

			auto& rendererContext = rendererRes->getRendererContext();
			auto& vma = rendererContext.getVMA();

			VkDeviceSize bufferSize{};
			auto componentsPack = sprites.getComponentsPack<Transform>();
			for (auto components : componentsPack)
			{
				bufferSize += components->getObjectsCapacity();
			}
			if (bufferSize <= 0)
				return Buffer{ nullptr };

			bufferSize *= sizeof(Transform);

			VkBufferCreateInfo createInfo
			{
				.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
				.size = bufferSize,
				.usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
				.sharingMode = VK_SHARING_MODE_EXCLUSIVE
			};

			Buffer buffer{ nullptr };
			if (!buffer.create(vma, createInfo))
			{
				Logger->error("Couldn't create transform buffer");
			}

			return buffer;
		}

	}
}