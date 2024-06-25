#pragma once

#include "Zinet/Math/ZtMathConfig.hpp"
#include "Zinet/Math/ZtVecTypes.hpp"

#include "Zinet/Core/ZtLogger.hpp"

#include <type_traits>
#include <array>

namespace zt 
{
	class ZINET_MATH_LAYER_API Math
	{
	public:

		inline static zt::core::ConsoleLogger Logger = zt::core::ConsoleLogger::Create("Math");

		Math() = delete;
		Math(const Math& other) = delete;
		Math(Math&& other) = delete;

		Math& operator = (const Math& other) = delete;
		Math& operator = (Math&& other) = delete;

		~Math() noexcept = delete;

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

		void INeedGeneratedLib() const;
	};

	inline std::uint32_t Math::GetMaximumMipmapLevelsCount(const Vector2ui& textureSize)
	{
		std::uint32_t mipmapLevels = static_cast<std::uint32_t>(std::floor(std::log2(std::max(textureSize.x, textureSize.y)))) + 1;
		return mipmapLevels;
	}
}