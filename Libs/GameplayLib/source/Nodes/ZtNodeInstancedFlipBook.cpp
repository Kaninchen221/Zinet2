#include "Zinet/Gameplay/Nodes/ZtNodeInstancedFlipBook.hpp"
#include "Zinet/Gameplay/Systems/ZtSystemFlipBooks.hpp"

namespace zt::gameplay
{
	vulkan_renderer::Transform& NodeInstancedFlipBook::getTransform() noexcept
	{
		return systemFlipBooks->getTransforms()[id];
	}

	const vulkan_renderer::Transform& NodeInstancedFlipBook::getTransform() const noexcept
	{
		return systemFlipBooks->getTransforms()[id];
	}

	FlipBookFrames& NodeInstancedFlipBook::getFrames() noexcept
	{
		return systemFlipBooks->getFlipBooksFrames()[id];
	}

	const FlipBookFrames& NodeInstancedFlipBook::getFrames() const noexcept
	{
		return systemFlipBooks->getFlipBooksFrames()[id];
	}
}