#pragma once

#include <gtest/gtest.h>

#include "Zinet/Core/ECS/ZtEntity.hpp"

#include "Zinet/Core/Tests/ZtTestTypes.hpp"

namespace zt::core::ecs::tests
{
	class ECSEntityTests : public ::testing::Test
	{
	protected:

		Entity entity{ 0 };

	};

	TEST_F(ECSEntityTests, PassTest)
	{

	}
}