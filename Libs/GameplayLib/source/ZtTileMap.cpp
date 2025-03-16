#include "Zinet/GameplayLib/ZtTileMap.hpp"

namespace zt::gameplay_lib
{
	sf::DrawInfo TileMap::getDrawInfo() const
	{
		sf::DrawInfo drawInfo;

		if (tilesCount.x * tilesCount.y != tiles.size())
		{
			Logger->error("You didn't pass enough tiles IDs for entire tile map");
			return drawInfo;
		}

		const Vector2f resoultion = texture.getResolution();
		if (!texture.isValid())
		{
			Logger->error("Texture is invalid");
			return drawInfo;
		}

		drawInfo.drawMode = software_renderer::DrawMode::Triangles,
		drawInfo.shaderProgram.fragmentShader.textures.push_back(texture);
		drawInfo.shaderProgram.fragmentShader.processFragment = sf::FragmentShaderSampleTextureProcess;

		drawInfo.vertices.reserve(tiles.size() * 4);
		drawInfo.indices.reserve(tiles.size() * 6);
		const Vector2f vertexOffset{ 1.f / tilesCount.x, 1.f / tilesCount.y };

		Vector2ui tileIndex = { 0.f, 0.f };
		auto tileSetTileCoordsIt = tiles.begin();
		for (tileIndex.y = 0; tileIndex.y < tilesCount.y; tileIndex.y++)
		{
			for (tileIndex.x = 0; tileIndex.x < tilesCount.x; tileIndex.x++)
			{
				const std::uint32_t indicesOffset = static_cast<std::uint32_t>(drawInfo.vertices.size());
				std::vector<std::uint32_t> newIndices
				{
					0 + indicesOffset, 1 + indicesOffset, 3 + indicesOffset,
					1 + indicesOffset, 2 + indicesOffset, 3 + indicesOffset
				};
				drawInfo.indices.insert(drawInfo.indices.end(), newIndices.begin(), newIndices.end());

				auto tileSetTileCoords = *tileSetTileCoordsIt;

				const Vector2f uvMin = { (tileSizeInTexture.x * tileSetTileCoords.x) / resoultion.x, (tileSizeInTexture.y * tileSetTileCoords.y) / resoultion.y };
				const Vector2f uvMax = { ((tileSizeInTexture.x * tileSetTileCoords.x) + tileSizeInTexture.x) / resoultion.x, ((tileSizeInTexture.y * tileSetTileCoords.y) + tileSizeInTexture.y) / resoultion.y };

				const Vector2f posMin = { tileIndex.x * vertexOffset.x, tileIndex.y * vertexOffset.y };
				const Vector2f posMax = { vertexOffset.x + (tileIndex.x * vertexOffset.x), vertexOffset.y + (tileIndex.y * vertexOffset.y) };

				drawInfo.vertices.push_back(sf::Vertex{ { posMin.x, posMin.y }, sf::WhiteColor, { uvMin.x, uvMin.y } });
				drawInfo.vertices.push_back(sf::Vertex{ { posMax.x, posMin.y }, sf::WhiteColor, { uvMax.x, uvMin.y } });
				drawInfo.vertices.push_back(sf::Vertex{ { posMax.x, posMax.y }, sf::WhiteColor, { uvMax.x, uvMax.y } });
				drawInfo.vertices.push_back(sf::Vertex{ { posMin.x, posMax.y }, sf::WhiteColor, { uvMin.x, uvMax.y } });

				++tileSetTileCoordsIt;
			}
		}

		drawInfo.shaderProgram.vertexShader.size = size;
		drawInfo.shaderProgram.vertexShader.position = position;
		drawInfo.shaderProgram.vertexShader.processVertex = sf::VertexShaderVerticesToWorldProcess;

		return drawInfo;
	}
}