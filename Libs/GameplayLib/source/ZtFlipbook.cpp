#include "Zinet/GameplayLib/ZtFlipbook.hpp"

#include "Zinet/Math/ZtMath.hpp"

namespace zt::gameplay_lib
{

	void Flipbook::addFrame(const FlipbookFrame& frame)
	{
		frames.push_back(frame);
	}

	void Flipbook::tick(float deltaTime)
	{
		while (deltaTime > 0.f)
		{
			deltaTime = updateFrame(deltaTime);
		}
	}

	sf::DrawInfo Flipbook::getDrawInfo() const
	{
		const FlipbookFrame& currentFrame = frames[currentFrameIndex];

		const Vector2f uvMin = { currentFrame.textureRegion.offset.x, currentFrame.textureRegion.offset.y };
		const Vector2f uvMax =
		{ 
			uvMin.x + currentFrame.textureRegion.size.x,
			uvMin.y + currentFrame.textureRegion.size.y
		};

		sf::DrawInfo drawInfo
		{
			.drawMode = software_renderer::DrawMode::Triangles,
			.vertices = std::vector<sf::Vertex>{
				sf::Vertex{ { .0f, .0f }, sf::WhiteColor, { uvMin.x, uvMin.y } },
				sf::Vertex{ { 1.f, .0f }, sf::WhiteColor, { uvMax.x, uvMin.y } },
				sf::Vertex{ { 1.f, 1.f }, sf::WhiteColor, { uvMax.x, uvMax.y } },
				sf::Vertex{ { .0f, 1.f }, sf::WhiteColor, { uvMin.x, uvMax.y } }
			},
			.indices = { 0, 1, 3, 1, 2, 3 }
		};

		drawInfo.shaderProgram.vertexShader.size = size;
		drawInfo.shaderProgram.vertexShader.position = getAbsolutePosition();
		drawInfo.shaderProgram.vertexShader.processVertex = sf::VertexShaderVerticesToWorldProcess;

		drawInfo.shaderProgram.fragmentShader.textures.push_back(currentFrame.texture);
		drawInfo.shaderProgram.fragmentShader.processFragment = sf::FragmentShaderSampleTextureProcess;

		return drawInfo;
	}

	float Flipbook::updateFrame(float deltaTime)
	{
		const FlipbookFrame& currentFrame = frames[currentFrameIndex];

		elapsedTime += deltaTime;
		if (elapsedTime >= currentFrame.time)
		{
			// Set next frame
			currentFrameIndex++;
			elapsedTime = 0.f;
			if (currentFrameIndex >= frames.size())
			{
				currentFrameIndex = 0;
			}
		}

		deltaTime -= currentFrame.time;
		if (deltaTime < 0.f)
			return 0.f;

		return deltaTime;
	}

}