#pragma once

#include "Zinet/Gameplay/Systems/ZtSystemTickable.hpp"

#include "Zinet/Core/ZtObjectsStorage.hpp"

#include <gtest/gtest.h>

namespace zt::gameplay::system::tests
{
	class TickableTests : public ::testing::Test
	{
	protected:

		void SetUp() override
		{
		}

		void TearDown() override
		{
		}

	};

	TEST_F(TickableTests, PassTest)
	{
	}
}