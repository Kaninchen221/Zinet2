#include "Zinet/Math/ZtMath.hpp"

namespace zt::math
{

	void GenerateLib()
	{

	}

	std::uint32_t GetMaximumMipmapLevelsCount(const Vector2ui& textureSize)
	{
		std::uint32_t mipmapLevels = static_cast<std::uint32_t>(std::floor(std::log2(std::max(textureSize.x, textureSize.y)))) + 1;
		return mipmapLevels;
	}

	float Distance(const Vector2f& vector1, const Vector2f& vector2)
	{
		return static_cast<float>(std::sqrt(std::pow(vector2.x - vector1.x, 2) + std::pow(vector2.y - vector1.y, 2)));
	}

	bool IsInsideRect(const RectF& rect, const Vector2f& position)
	{
		const auto& min = rect.offset;
		const auto max = min + rect.size;

		return position.x > min.x && position.x < max.x && position.y > min.y && position.y < max.y;
	}
}