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
			ecs::ConstResource<VulkanRenderer> rendererRes,
			ecs::ConstResource<core::AssetsStorage> assetsStorageRes)
		{
			if (!rendererRes)
			{
				worldCommands.addResource(ExitReason{ true, "Expected rendererRes" });
				return;
			}

			if (!rendererRes->isInitialized())
			{
				worldCommands.addResource(ExitReason{ true, "Expected initialized renderer" });
				return;
			}

			if (!assetsStorageRes)
			{
				worldCommands.addResource(ExitReason{ true, "Expected assetsStorageRes" });
				return;
			}

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

			std::array<DescriptorInfo, 2> descriptorInfos;
			/// Add descriptor infos

			// shader modules
			// descriptors
// 			GraphicsPipelineCreateInfo createInfo
// 			{
// 				.rendererContext = rendererRes->getRendererContext(),
// 				.shaderModules = std::move(shaderModules),
// 			};
// 
// 			GraphicsPipeline graphicsPipeline;
// 			graphicsPipeline.create(createInfo);

// 			worldCommands.spawn(
// 				std::move(graphicsPipeline)
// 			);

			// Destroy shader modules
			{
				auto& device = rendererRes->getRendererContext().getDevice();
				vertexShaderModule.destroy(device);
				fragmentShaderModule.destroy(device);
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
					worldCommands.addResource(ExitReason{ true, "Couldn't load vertex shader asset" });
					return { nullptr };
				}
			}

			ShaderModule shaderModule{ nullptr };
			if (!shaderModule.create(renderer.getRendererContext().getDevice(), shader->getCompileResult()))
			{
				worldCommands.addResource(ExitReason{ true, "Couldn't create the vertex shader module" });
				return { nullptr };
			}

			return shaderModule;
		}

// 		DescriptorInfo Sprites::CreateDescriptorInfo()
// 		{
// 			if (!transformsMatricesBuffer.isValid())
// 				return {};
// 
// 			DescriptorInfo result;
// 
// 			result.buffersPacks.emplace_back().buffersPerType[VK_DESCRIPTOR_TYPE_STORAGE_BUFFER].push_back(&transformsMatricesBuffer);
// 
// 			if (!assetTexture.isValid() || !assetTexture->isLoaded())
// 			{
// 				Logger->error("AssetTexture is invalid");
// 				return result;
// 			}
// 
// 			auto& sampler = assetTexture->sampler;
// 			if (!sampler.isValid() || !sampler->isLoaded())
// 			{
// 				Logger->error("Sampler of texture is invalid");
// 				return result;
// 			}
// 
// 			result.texturesInfos =
// 			{
// 				vulkan_renderer::TextureInfo
// 				{
// 					.texture = &assetTexture->texture,
// 					.sampler = &sampler.get()->sampler,
// 					.shaderType = vulkan_renderer::ShaderType::Fragment
// 				}
// 			};
// 		}

	}
}