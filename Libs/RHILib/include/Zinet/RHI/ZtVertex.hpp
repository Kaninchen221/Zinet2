#pragma once

#include "Zinet/RHI/ZtRHIConfig.hpp"
#include "Zinet/RHI/ZtTypes.hpp"

#include "Zinet/Math/ZtVecTypes.hpp"

namespace zt::rhi
{

	class ZINET_RHI_API Vertex
	{
	public:
		Vertex() = default;
		Vertex(const Vertex& other) = default;
		Vertex(Vertex&& other) = default;

		Vertex& operator = (const Vertex& other) = default;
		Vertex& operator = (Vertex&& other) = default;

		~Vertex() noexcept = default;

		void setPosition(const Vector4f& newPosition) { position = newPosition; }
		const Vector4f& getPosition() const { return position; }

		void setColor(const Vector4f& newColor) { color = newColor; }
		const Vector4f& getColor() const { return color; }

		void setUV(const Vector2f& newUV) { uv = newUV; }
		const Vector2f& getUV() const { return uv; }

	protected:

		Vector4f position;
		Vector4f color;
		Vector2f uv;
	};

}