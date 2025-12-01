#pragma once

#include "Zinet/Gameplay/Systems/ZtSystemScript.hpp"

#include <gtest/gtest.h>

namespace zt::gameplay::systems::tests
{
	class ScriptTests : public ::testing::Test
	{
	protected:

		void SetUp() override
		{
		}

		void TearDown() override
		{
		}

	};

	TEST_F(ScriptTests, PassTest)
	{
	}
}