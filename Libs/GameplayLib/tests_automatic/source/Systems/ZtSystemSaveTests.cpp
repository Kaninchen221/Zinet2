#pragma once

#include "Zinet/Gameplay/Systems/ZtSystemSave.hpp"

#include <gtest/gtest.h>

namespace zt::gameplay::tests
{
	class SystemSaveTests : public ::testing::Test
	{
	protected:

		void SetUp() override
		{
		}

		void TearDown() override
		{
		}
	};

	TEST_F(SystemSaveTests, PassTest)
	{
	}
}