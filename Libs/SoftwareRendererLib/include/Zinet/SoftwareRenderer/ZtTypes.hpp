#pragma once

#include "Zinet/SoftwareRenderer/ZtSoftwareRendererConfig.hpp"

#include "Zinet/Math/ZtVecTypes.hpp"

#include <algorithm>
#include <functional>

namespace zt::software_renderer
{
	using Color = Vector4<std::uint8_t>;
	constexpr const Vector4<std::uint8_t> RedColor{ 255, 0, 0, 255 };
	constexpr const Vector4<std::uint8_t> GreenColor{ 0, 255, 0, 255 };
	constexpr const Vector4<std::uint8_t> BlueColor{ 0, 0, 255, 255 };
	constexpr const Vector4<std::uint8_t> BlackColor{ 0, 0, 0, 255 };
	constexpr const Vector4<std::uint8_t> WhiteColor{ 255, 255, 255, 255 };
	constexpr const Vector4<std::uint8_t> ZeroColor{ 0, 0, 0, 0 };

	struct ZINET_SOFTWARE_RENDERER_API Vertex
	{
		Vector2f position;
		Vector4f color;
		Vector2f uv;
	};

	struct ZINET_SOFTWARE_RENDERER_API Triangle
	{
		Vertex v1;
		Vertex v2;
		Vertex v3;
	};

	struct ZINET_SOFTWARE_RENDERER_API Pixel
	{
		Vector2<std::int16_t> coords;
		Color color;
		Vector2f uv;
	};

	enum class ColorFormat
	{
		R8G8B8A8_SRGB
	};

	enum class DrawMode
	{
		Points,
		TrianglesLines,
		Triangles,
		Lines
	};
}