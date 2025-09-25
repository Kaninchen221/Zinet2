#include "Zinet/Gameplay/Systems/ZtSystemFlipBooks.hpp"

namespace zt::gameplay
{
	void SystemFlipBooks::update()
	{
		SystemSprites::update();
	}

	bool SystemFlipBooks::deinit()
	{
		auto systemSpritesDeinit = SystemSprites::deinit();
		return systemSpritesDeinit;
	}

	void SystemFlipBooks::addNode(const ObjectWeakHandle<Node>& node)
	{
		SystemSprites::addNode(node);
	}

	vulkan_renderer::DescriptorInfo SystemFlipBooks::getDescriptorInfo()
	{
		auto result = SystemSprites::getDescriptorInfo();
		return result;
	}

}