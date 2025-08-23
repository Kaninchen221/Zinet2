#include "Zinet/Gameplay/Nodes/ZtNodeSprite.hpp"

#include "Zinet/VulkanRenderer/ZtDrawInfo.hpp"

#include "Zinet/Core/ZtDebug.hpp"

namespace zt::gameplay
{
	void NodeSprite::show()
	{
		Node2D::show();
		texture.show();
	}

	vulkan_renderer::DescriptorInfo NodeSprite::getPipelineDescriptorInfos()
	{
		auto descriptorInfo = [](AssetTexture* assetTexture) -> vulkan_renderer::DescriptorInfo
		{
			return
			{
				.uniformBuffers = {},
				.texturesInfos =
				{
					vulkan_renderer::TextureInfo
					{
						.texture = &assetTexture->texture,
						.sampler = &assetTexture->sampler.get()->sampler,
						.shaderType = vulkan_renderer::ShaderType::Fragment
					}
				}
			};
		};

		if (!texture || !texture->isLoaded())
		{
			if (!DefaultTexture || !DefaultTexture->isLoaded())
			{
				Ensure(false); // Node2D should have always a valid default texture
				TerminateDebug();
			}

			return descriptorInfo(DefaultTexture.get());
		}

		return descriptorInfo(texture.get());
	}

}