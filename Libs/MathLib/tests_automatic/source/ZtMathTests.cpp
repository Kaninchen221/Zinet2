#pragma once

#include "Zinet/Math/ZtMath.hpp"

#include "Zinet/Core/ZtTypeTraits.hpp"

#include <gtest/gtest.h>

namespace zt::math::tests
{
	class MathTests : public ::testing::Test
	{
	protected:

		void compareValues(auto vector, auto array)
		{
			for (int index = 0; index < vector.length(); ++index)
			{
				EXPECT_EQ(vector[index], array[index]);
			}
		};

		static_assert(!std::is_default_constructible_v<Math>);
		static_assert(!std::is_copy_constructible_v<Math>);
		static_assert(!std::is_move_constructible_v<Math>);
		static_assert(!std::is_copy_assignable_v<Math>);
		static_assert(!std::is_move_assignable_v<Math>);
		static_assert(!std::is_destructible_v<Math>);
	};

	TEST_F(MathTests, FromVectorToArray)
	{
		Vector2f vec2f = { 23.f, 1.f };
		std::array<float, 2> array2f = { 0.f, 0.f };
		array2f = Math::FromVectorToArray(vec2f);
		compareValues(vec2f, array2f);

		Vector3d vec3d = { 23.0, 1.0, 2333.232 };
		std::array<double, 3> array3d = { 0.0, 0.0, 0.0 };
		array3d = Math::FromVectorToArray(vec3d);
		compareValues(vec3d, array3d);
	}

	TEST_F(MathTests, FromArrayToVector)
	{
		std::array<float, 2> array2f = { 23.f, 1.f };
		Vector2f vec2f = { 0.f, 0.f };
		vec2f = Math::FromArrayToVector(array2f);
		compareValues(vec2f, array2f);

		std::array<double, 3> array3d = { 23.0, 1.0, 2333.232 };
		Vector3d vec3d = { 0.0, 0.0, 0.0 };
		vec3d = Math::FromArrayToVector(array3d);
		compareValues(vec3d, array3d);
	}

	TEST_F(MathTests, AABBTest)
	{
		RectF rect
		{
			.offset = { 100.f, 100.f },
			.size = { 200.f, 200.f }
		};
		Vector2f position = { 150.f, 200.f };
		bool result = Math::IsInsideRect(rect, position);
		ASSERT_TRUE(result);
	}
}
