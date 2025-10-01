#pragma once

#include <gtest/gtest.h>

#include "Zinet/Core/ECS/ZtEntity.hpp"

namespace zt::core::ecs
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