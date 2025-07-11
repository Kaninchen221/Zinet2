#pragma once

#include "Zinet/Math/ZtMathConfig.hpp"
#include "Zinet/Math/ZtVecTypes.hpp"

#include "Zinet/Core/ZtLogger.hpp"

#include <type_traits>
#include <array>
#include <math.h>

namespace zt 
{
	class ZINET_MATH_LAYER_API Math
	{
	public:

		inline static zt::core::ConsoleLogger Logger = zt::core::ConsoleLogger::Create("Math");

		inline static const constexpr auto EpsilonF = std::numeric_limits<float>::epsilon();
		inline static const constexpr auto EpsilonD = std::numeric_limits<double>::epsilon();

		Math() = delete;
		Math(const Math& other) = delete;
		Math(Math&& other) = delete;

		Math& operator = (const Math& other) = delete;
		Math& operator = (Math&& other) = delete;

		~Math() ZINET_API_POST = delete;

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

		void INeedGeneratedLib() const;
	};

	inline std::uint32_t Math::GetMaximumMipmapLevelsCount(const Vector2ui& textureSize)
	{
		std::uint32_t mipmapLevels = static_cast<std::uint32_t>(std::floor(std::log2(std::max(textureSize.x, textureSize.y)))) + 1;
		return mipmapLevels;
	}

	inline float Math::Distance(const Vector2f& vector1, const Vector2f& vector2)
	{
		return static_cast<float>(std::sqrt(std::pow(vector2.x - vector1.x, 2) + std::pow(vector2.y - vector1.y, 2)));
	}

	inline bool Math::IsInsideRect(const RectF& rect, const Vector2f& position)
	{
		const auto& min = rect.offset;
		const auto max = min + rect.size;

		return position.x > min.x && position.x < max.x && position.y > min.y && position.y < max.y;
	}
}