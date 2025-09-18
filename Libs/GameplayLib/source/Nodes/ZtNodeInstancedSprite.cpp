#include "Zinet/Gameplay/Nodes/ZtNodeInstancedSprite.hpp"
#include "Zinet/Gameplay/Systems/ZtSystemSprites.hpp"

namespace zt::gameplay
{
	vulkan_renderer::Transform& NodeInstancedSprite::getTransform() noexcept
	{
		return systemSprites->getTransforms()[id];
	}

	const vulkan_renderer::Transform& NodeInstancedSprite::getTransform() const noexcept
	{
		return systemSprites->getTransforms()[id];
	}

}