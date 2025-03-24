#pragma once

#include "Zinet/GameplayLib/ZtGameplayLibConfig.hpp"
#include "Zinet/GameplayLib/ZtNode.hpp"

#include "Zinet/Core/Reflection/ZtReflection.hpp"
#include "Zinet/Core/ZtLogger.hpp"

#include "Zinet/SoftwareRenderer/ZtRenderTarget.hpp"

#include "Zinet/Math/ZtVecTypes.hpp"

namespace zt::gameplay_lib
{
	namespace sf = software_renderer;

	ZT_REFLECT_CLASS()
		class ZINET_GAMEPLAY_LIB_API TileMap : public Node
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("TileMap");

	public:

		sf::DrawInfo getDrawInfo() const override;

	protected:

		ZT_REFLECT_MEMBER(ReadWrite)
		std::vector<Vector2ui> tiles;

		ZT_REFLECT_MEMBER(ReadWrite)
		Vector2ui tilesCount = { 0, 0 };

		ZT_REFLECT_MEMBER(ReadWrite)
		Vector2ui tileSizeInTexture = { 0, 0 };

		ZT_REFLECT_MEMBER(ReadWrite)
		sf::RenderTarget texture;

	public:

/*GENERATED_CODE_START*/
		static_assert(IsObjectClassInherited); // Class using ZT_REFLECT_CLASS should inherit public from Object class
		const inline static bool RegisterClassResult = RegisterClass<TileMap>();
		std::unique_ptr<ObjectBase> createCopy() const override { std::unique_ptr<ObjectBase> result = createCopyInternal<TileMap>(); *result = *this; return result; }
		
		TileMap() = default;
		TileMap(const TileMap& other) = default;
		TileMap(TileMap&& other) = default;
		~TileMap() noexcept = default;
		
		TileMap& operator = (const TileMap& other) = default;
		TileMap& operator = (TileMap&& other) = default;
		
		class ClassInfo : public zt::core::ClassInfoBase
		{
		public:
		
			std::string_view getClassName() const override { return "TileMap"; }
		};
		const zt::core::ClassInfoBase* getClassInfo() const override { static ClassInfo classInfo; return &classInfo; }
		
		
		const decltype(tiles)& getTiles() const { return tiles; }
		decltype(tiles)& getTiles() { return tiles; }
		void setTiles(const decltype(tiles)& newValue) { tiles = newValue; }
		
		const decltype(tilesCount)& getTilesCount() const { return tilesCount; }
		decltype(tilesCount)& getTilesCount() { return tilesCount; }
		void setTilesCount(const decltype(tilesCount)& newValue) { tilesCount = newValue; }
		
		const decltype(tileSizeInTexture)& getTileSizeInTexture() const { return tileSizeInTexture; }
		decltype(tileSizeInTexture)& getTileSizeInTexture() { return tileSizeInTexture; }
		void setTileSizeInTexture(const decltype(tileSizeInTexture)& newValue) { tileSizeInTexture = newValue; }
		
		const decltype(texture)& getTexture() const { return texture; }
		decltype(texture)& getTexture() { return texture; }
		void setTexture(const decltype(texture)& newValue) { texture = newValue; }
		
/*GENERATED_CODE_END*/
	};
}