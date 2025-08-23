#include "Zinet/Gameplay/Nodes/ZtNodeSprite.hpp"

#include "Zinet/VulkanRenderer/ZtDrawInfo.hpp"

namespace zt::gameplay
{
	void NodeSprite::show()
	{
		Node2D::show();
		texture.show();
	}

	vulkan_renderer::DescriptorInfo NodeSprite::getPipelineDescriptorInfos()
	{
		if (!texture || !texture->isLoaded())
			return {};

		return vulkan_renderer::DescriptorInfo
		{
			.uniformBuffers = {},
			.texturesInfos =
			{
				vulkan_renderer::TextureInfo
				{
					.texture = &texture->texture,
					.sampler = &texture->sampler.get()->sampler,
					.shaderType = vulkan_renderer::ShaderType::Fragment
				}
			}
		};
	}

}