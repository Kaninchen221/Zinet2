#include "Zinet/GameplayLib/ZtHorizontalSlidingWorld.hpp"
#include "Zinet/GameplayLib/ZtGameplayLoop.hpp"

#include "Zinet/Core/ZtPaths.hpp"

namespace zt::gameplay_lib
{

	bool HorizontalSlidingWorld::init(std::filesystem::path contentFolderPath, std::weak_ptr<HorizontalSlidingWorld> newSelfWeakPtr)
	{
		selfWeakPtr = newSelfWeakPtr;

		const std::filesystem::path path = contentFolderPath / "ground_tile_set.png";
		if (!groundSliceTexture.loadFromFilePNG(path))
		{
			Logger->error("Can't load ground tile slice texture {}", path.generic_string());
			return false;
		}

		for (std::int32_t i = 0; i < slicesCount; i++)
		{
			addGroundSlice();
		}

		return true;
	}

	void HorizontalSlidingWorld::tick(float deltaTime)
	{
		position.x -= deltaTime * slidingSpeed;

		if (groundSlicesNodes.front()->getAbsolutePosition().x < deadEndX)
		{
			groundSlicesNodes.erase(groundSlicesNodes.begin());
			addGroundSlice();
		}
	}

	void HorizontalSlidingWorld::addGroundSlice()
	{
		std::shared_ptr<TileMap> sliceNode = std::make_shared<TileMap>();

		Vector2f groundSlicePos = Vector2f{ 0.f, 0.f };
		if (!groundSlicesNodes.empty())
		{
			const auto& lastSliceNode = groundSlicesNodes.back();
			groundSlicePos = lastSliceNode->getPosition();
			groundSlicePos.x += lastSliceNode->getSize().x;
		}

		sliceNode->setPosition(groundSlicePos);
		sliceNode->setTexture(groundSliceTexture);
		sliceNode->setTilesCount({ 1, 3 });
		sliceNode->setTiles({
			{ 1, 0 },
			{ 1, 1 },
			{ 1, 2 }
			});
		sliceNode->setTileSizeInTexture({ 16, 16 });
		sliceNode->setSize(sliceSize);
		sliceNode->setParentNode(selfWeakPtr);

		groundSlicesNodes.push_back(sliceNode);
		if (auto gameplayLoop = GameplayLoop::Get())
			gameplayLoop->drawableSystem.addNode(sliceNode);
	}

}