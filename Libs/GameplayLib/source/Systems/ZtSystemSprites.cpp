#include "Zinet/Gameplay/Systems/ZtSystemSprites.hpp"

#include "Zinet/Gameplay/Assets/ZtAssetShader.hpp"

#include "Zinet/VulkanRenderer/ZtGraphicsPipeline.hpp"
#include "Zinet/VulkanRenderer/ZtVulkanRenderer.hpp"
#include "Zinet/VulkanRenderer/ZtShaderModule.hpp"

#include "Zinet/Core/ZtExitReason.hpp"

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
			ecs::ConstResource<core::AssetsStorage> assetsStorageRes)
		{
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

			if (!assetsStorageRes)
			{
				worldCommands.addResource(ExitReason{ "Expected assetsStorageRes" });
				return;
			}

			auto transformBuffer = CreateTransformBuffer(rendererRes, sprites);

			std::map<ShaderType, const ShaderModule*> shaderModules;

			// Shader Modules
			auto vertexShaderModule = CreateShaderModule(worldCommands, *rendererRes, *assetsStorageRes, "Content/Shaders/shader_sprites.vert");
			if (!vertexShaderModule)
				return;
			
			shaderModules.insert({ ShaderType::Vertex, &vertexShaderModule });

			auto fragmentShaderModule = CreateShaderModule(worldCommands, *rendererRes, *assetsStorageRes, "Content/Shaders/shader_sprites.frag");
			if (!fragmentShaderModule)
				return;
			
			shaderModules.insert({ ShaderType::Fragment, &fragmentShaderModule });

			worldCommands.spawn(Sprite{}, std::move(transformBuffer));

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
			const AssetsStorage& assetsStorage,
			std::string assetShaderKey)
		{
			auto shader = assetsStorage.getAs<asset::Shader>(assetShaderKey);

			if (!shader->isLoaded())
			{
				if (!shader->load(Paths::RootPath()))
				{
					worldCommands.addResource(ExitReason{ "Couldn't load vertex shader asset" });
					return { nullptr };
				}
			}

			ShaderModule shaderModule{ nullptr };
			if (!shaderModule.create(renderer.getRendererContext().getDevice(), shader->getCompileResult()))
			{
				worldCommands.addResource(ExitReason{ "Couldn't create the vertex shader module" });
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