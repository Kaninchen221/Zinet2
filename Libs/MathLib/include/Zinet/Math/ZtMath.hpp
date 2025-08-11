#pragma once

#include "Zinet/Math/ZtMathConfig.hpp"
#include "Zinet/Math/ZtVecTypes.hpp"

#include <type_traits>
#include <array>
#include <math.h>

namespace zt::math
{
	ZINET_MATH_API void GenerateLib();

	inline static const constexpr auto EpsilonF = std::numeric_limits<float>::epsilon();
	inline static const constexpr auto EpsilonD = std::numeric_limits<double>::epsilon();

	template<typename VectorType, typename ArrayType = std::array<typename VectorType::value_type, VectorType::length()>>
	constexpr static ArrayType FromVectorToArray(const VectorType& vector)
	{
		ArrayType result{};

		for (int index = 0; index < VectorType::length(); ++index)
		{
			result[index] = vector[index];
		}

		return result;
	}

	template<typename ArrayType, typename VectorType = glm::vec<std::tuple_size_v<ArrayType>, typename ArrayType::value_type, glm::defaultp>>
	constexpr static VectorType FromArrayToVector(const ArrayType& array)
	{
		VectorType result{};

		for (int index = 0; index < VectorType::length(); ++index)
		{
			result[index] = array[index];
		}

		return result;
	}

	static std::uint32_t GetMaximumMipmapLevelsCount(const Vector2ui& textureSize);

	static float Distance(const Vector2f& vector1, const Vector2f& vector2);

	static bool IsInsideRect(const RectF& rect, const Vector2f& position);

	inline std::uint32_t GetMaximumMipmapLevelsCount(const Vector2ui& textureSize)
	{
		std::uint32_t mipmapLevels = static_cast<std::uint32_t>(std::floor(std::log2(std::max(textureSize.x, textureSize.y)))) + 1;
		return mipmapLevels;
	}

	inline float Distance(const Vector2f& vector1, const Vector2f& vector2)
	{
		return static_cast<float>(std::sqrt(std::pow(vector2.x - vector1.x, 2) + std::pow(vector2.y - vector1.y, 2)));
	}

	inline bool IsInsideRect(const RectF& rect, const Vector2f& position)
	{
		const auto& min = rect.offset;
		const auto max = min + rect.size;

		return position.x > min.x && position.x < max.x && position.y > min.y && position.y < max.y;
	}
}