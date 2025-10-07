#pragma once

#include <gtest/gtest.h>

#include "Zinet/Core/ECS/ZtQuery.hpp"
#include "Zinet/Core/ECS/ZtTypeLessVector.hpp"

#include "Zinet/Core/Tests/ZtTestTypes.hpp"

namespace zt::core::ecs::tests
{
	class ECSQueryTests : public ::testing::Test
	{
	protected:

		std::vector<QueryTypes::Segment> segments;
		QueryTypes::ComponentsPack componentsPack;
		Query<Sprite> query{ {} };

		void SetUp() override;
	};

	TEST_F(ECSQueryTests, Test)
	{
		ASSERT_EQ(query.getComponentsCount(), 6);
	}

	TEST_F(ECSQueryTests, ForRangeBasedLoopTest)
	{
		size_t count = 0;
		for (const auto& component : query)
		{
			ASSERT_EQ(component.id, count);
			++count;
		}

		ASSERT_EQ(count, 6);
	}

	void ECSQueryTests::SetUp()
	{
		segments.reserve(3);
		componentsPack.reserve(3);

		TypeLessVector& vector1 = segments.emplace_back(TypeLessVector::Create<Sprite>());
		vector1.add(Sprite{ 0 });
		vector1.add(Sprite{ 1 });
		vector1.add(Sprite{ 2 });

		TypeLessVector& vector2 = segments.emplace_back(TypeLessVector::Create<Sprite>());
		vector2.add(Sprite{ 3 });
		vector2.add(Sprite{ 4 });

		TypeLessVector& vector3 = segments.emplace_back(TypeLessVector::Create<Sprite>());
		vector3.add(Sprite{ 5 });

		componentsPack.push_back(&vector1);
		componentsPack.push_back(&vector2);
		componentsPack.push_back(&vector3);

		query = Query<Sprite>(componentsPack);
	}
}