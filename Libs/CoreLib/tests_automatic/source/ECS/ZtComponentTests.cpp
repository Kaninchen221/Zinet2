#pragma once

#include <gtest/gtest.h>

#include "Zinet/Core/ECS/ZtComponent.hpp"

namespace zt::core::ecs
{
	class ECSComponentTests : public ::testing::Test
	{
	protected:

		Component component;

	};

	TEST_F(ECSComponentTests, Test)
	{

	}
}