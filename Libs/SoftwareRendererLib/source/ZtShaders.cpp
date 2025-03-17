#include "Zinet/SoftwareRenderer/ZtShaders.hpp"

namespace zt::software_renderer
{

	void VertexShaderVerticesToWorldProcess(const VertexShader& vertexShader, Vertex& vertex)
	{
		Vector2f sizeScale = vertexShader.size / vertexShader.cameraSize;
		vertex.position *= sizeScale;
		vertex.position += (vertexShader.position - vertexShader.cameraPosition) / vertexShader.cameraSize;
		vertex.position += (vertexShader.cameraSize / 2.f) / vertexShader.cameraSize;
	}

	void FragmentShaderSampleTextureProcess(const FragmentShader& fragmentShader, Pixel& fragment)
	{
		const auto& uv = fragment.uv;
		fragment.color = SampleTexture(fragmentShader.textures[0], uv);
		if (fragment.color == ZeroColor)
			fragment.color = fragmentShader.sourceColor;
	};

}