#include "Zinet/GameplayLib/ZtSprite.hpp"

namespace zt::gameplay_lib
{
	sf::DrawInfo Sprite::getDrawInfo() const
	{
		const Vector2f uvMin = { textureRegion.offset.x, textureRegion.offset.y };
		const Vector2f uvMax =
		{ 
			uvMin.x + textureRegion.size.x,
			uvMin.y + textureRegion.size.y
		};

		sf::DrawInfo drawInfo
		{
			.drawMode = software_renderer::DrawMode::Triangles,
			.vertices = std::vector<sf::Vertex>{
				sf::Vertex{ { .4f, .0f }, sf::WhiteColor, { uvMin.x, uvMin.y } },
				sf::Vertex{ { .6f, .0f }, sf::WhiteColor, { uvMax.x, uvMin.y } },
				sf::Vertex{ { .6f, .2f }, sf::WhiteColor, { uvMax.x, uvMax.y } },
				sf::Vertex{ { .4f, .2f }, sf::WhiteColor, { uvMin.x, uvMax.y } }
			},
			.indices = { 0, 1, 3, 1, 2, 3 }
		};

		drawInfo.shaderProgram.fragmentShader.textures.push_back(texture);
		drawInfo.shaderProgram.fragmentShader.processFragment = sf::FragmentShaderSampleTextureProcess;

		return drawInfo;
	}
}