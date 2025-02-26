#pragma once

#include "Zinet/SoftwareRenderer/ZtSoftwareRendererConfig.hpp"

#include "Zinet/Math/ZtVecTypes.hpp"
#include <algorithm>

namespace zt::software_renderer
{
	struct ZINET_SOFTWARE_RENDERER_API Vertex
	{
		Vector2f position;
		Vector4f color;
	};

	struct ZINET_SOFTWARE_RENDERER_API Triangle
	{
		Vertex v1;
		Vertex v2;
		Vertex v3;
	};

	struct ZINET_SOFTWARE_RENDERER_API Pixel
	{
		Vector2ui coords;
		Color color;
	};

	enum class ColorFormat
	{
		R8G8B8A8_SRGB
	};

	enum class DrawMode
	{
		Points,
		Lines,
		Triangles
	};

	class ZINET_SOFTWARE_RENDERER_API VertexShader
	{
	public:

		// The vertex param is the input and the output
		void processVertex(Vertex& vertex) const { }

	};

	class ZINET_SOFTWARE_RENDERER_API FragmentShader
	{
	public:

		const Color* sourceColor = nullptr;

		// The fragment param is the input and the output
		void processFragment(Pixel& fragment) const {}

	};
}