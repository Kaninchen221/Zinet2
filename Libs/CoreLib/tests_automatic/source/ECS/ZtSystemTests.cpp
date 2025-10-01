#pragma once

#include <gtest/gtest.h>

#include "Zinet/Core/ECS/ZtSystem.hpp"

namespace zt::core::ecs
{
	class ECSSystemTests : public ::testing::Test
	{
	protected:

		System system;

	};

	TEST_F(ECSSystemTests, Test)
	{

	}
}