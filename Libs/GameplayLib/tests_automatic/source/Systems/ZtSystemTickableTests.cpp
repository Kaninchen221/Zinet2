#pragma once

#include "Zinet/Core/ECS/ZtWorld.hpp"
#include "Zinet/Core/ECS/ZtSchedule.hpp"

#include "Zinet/Gameplay/Systems/ZtSystemTickable.hpp"

#include <gtest/gtest.h>

namespace zt::gameplay::system::tests
{
	class TickableTests : public ::testing::Test
	{
	protected:

	};

	// TODO: Define the resources as private after adding this feature
	TEST_F(TickableTests, PassTest)
	{
		core::ecs::World world;
		core::ecs::Schedule schedule;

		schedule.runOneSystemOnce(Tickable{}, Tickable::Init, world);
		auto deltaTimeRes = world.getResource<DeltaTime>();

		ASSERT_TRUE(deltaTimeRes);
		ASSERT_EQ(*deltaTimeRes, 0);

		auto timePointRes = world.getResource<TickableData>();
		ASSERT_TRUE(timePointRes);

		for (size_t i = 0; i < 10; ++i)
		{
			schedule.runOneSystemOnce(Tickable{}, Tickable::Update, world);
			ASSERT_NE(*deltaTimeRes, 0);
		}
	}
}