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

		const std::filesystem::path path2 = contentFolderPath / "under_ground.png";
		if (!undergroundSliceTexture.loadFromFilePNG(path2))
		{
			Logger->error("Can't load ground tile slice texture {}", path.generic_string());
			return false;
		}

		return true;
	}

	void HorizontalSlidingWorld::tick(float deltaTime)
	{
		position.x -= deltaTime * slidingSpeed;

		if (groundSlicesNodes.empty())
		{
			for (std::int32_t i = 0; i < slicesCount; i++)
			{
				addGroundSlice();
				addUndergroundSlice();
			}
		}
		else if (groundSlicesNodes.front()->getAbsolutePosition().x < deadEndX)
		{
			groundSlicesNodes.erase(groundSlicesNodes.begin());
			addGroundSlice();

			undergroundSlicesNodes.erase(undergroundSlicesNodes.begin());
			addUndergroundSlice();
		}
	}

	void HorizontalSlidingWorld::addGroundSlice()
	{
		std::shared_ptr<TileMap> sliceNode = std::make_shared<TileMap>();

		Vector2f slicePositionOffset = Vector2f{ 0.f, 0.f };
		Vector2f slicePosition = Vector2f{ 0.f, 0.f } + slicePositionOffset;
		if (!groundSlicesNodes.empty())
		{
			const auto& lastSliceNode = groundSlicesNodes.back();
			slicePosition.x += lastSliceNode->getPosition().x + lastSliceNode->getSize().x;
		}

		sliceNode->setPosition(slicePosition);
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

	void HorizontalSlidingWorld::addUndergroundSlice()
	{
		std::shared_ptr<TileMap> sliceNode = std::make_shared<TileMap>();

		Vector2f slicePositionOffset = Vector2f{ 0.f, 90.f };
		Vector2f slicePosition = Vector2f{ 0.f, 0.f } + slicePositionOffset;
		if (!undergroundSlicesNodes.empty())
		{
			const auto& lastSliceNode = undergroundSlicesNodes.back();
			slicePosition.x += lastSliceNode->getPosition().x + lastSliceNode->getSize().x;
		}

		sliceNode->setPosition(slicePosition);
		sliceNode->setTexture(undergroundSliceTexture);
		sliceNode->setTilesCount({ 1, 3 });
		sliceNode->setTiles({
			{ 1, 0 },
			{ 1, 1 },
			{ 1, 2 }
			});
		sliceNode->setTileSizeInTexture({ 16, 16 });
		sliceNode->setSize(sliceSize);
		sliceNode->setParentNode(selfWeakPtr);

		undergroundSlicesNodes.push_back(sliceNode);
		if (auto gameplayLoop = GameplayLoop::Get())
			gameplayLoop->drawableSystem.addNode(sliceNode);
	}

}