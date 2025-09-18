#pragma once

#include "Zinet/Math/ZtMathConfig.hpp"
#include "Zinet/Math/ZtVecTypes.hpp"

#include <type_traits>
#include <array>
#include <math.h>

namespace zt::math
{
	ZINET_MATH_API void GenerateLib();

	inline static const constexpr float EpsilonF = std::numeric_limits<float>::epsilon();
	inline static const constexpr double EpsilonD = std::numeric_limits<double>::epsilon();

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

	inline std::uint32_t GetMaximumMipmapLevelsCount(const Vector2ui& textureSize);

	inline float Distance(const Vector2f& vector1, const Vector2f& vector2);

	inline bool IsInsideRect(const RectF& rect, const Vector2f& position);
	
}