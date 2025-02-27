#pragma once

#include "Zinet/SoftwareRenderer/ZtSoftwareRendererConfig.hpp"

#include "Zinet/Math/ZtVecTypes.hpp"

#include <algorithm>
#include <functional>

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
		TrianglesLines,
		Triangles
	};

	class ZINET_SOFTWARE_RENDERER_API VertexShader
	{
	public:

		using ProcessVertexCallableT = std::function<void(const VertexShader&, Vertex& vertex)>;

		ProcessVertexCallableT processVertex = nullptr;

		operator bool() const { return processVertex.operator bool(); }

	};

	class ZINET_SOFTWARE_RENDERER_API FragmentShader
	{
	public:

		const Color* sourceColor = nullptr;

		using ProcessFragmentCallableT = std::function<void(const FragmentShader&, Pixel& fragment)>;

		ProcessFragmentCallableT processFragment = nullptr;

		operator bool() const { return processFragment.operator bool(); }

	};
}