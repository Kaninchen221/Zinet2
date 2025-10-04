#pragma once

#include <gtest/gtest.h>

#include "Zinet/Core/ECS/ZtComponent.hpp"

#include "Zinet/Core/Tests/ZtTestTypes.hpp"

namespace zt::core::ecs::tests
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