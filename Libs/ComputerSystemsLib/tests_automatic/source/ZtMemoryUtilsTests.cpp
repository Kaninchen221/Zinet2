#pragma once

#include "Zinet/ComputerSystems/ZtMemoryUtils.hpp"

#include <gtest/gtest.h>
#include "Zinet/Core/ZtLogger.hpp"

namespace zt::computer_systems::tests
{
	using namespace memory_utils;

	class MemoryUtilsTests : public ::testing::Test
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("MemoryUtilsTests");

	};

	TEST_F(MemoryUtilsTests, ToHexStringTest)
	{
		EXPECT_EQ(ToHexString(uint8_t(255u)), "ff");
		EXPECT_EQ(ToHexString(uint8_t(0u)), "00");
		EXPECT_EQ(ToHexString(uint8_t(1u)), "01");
		EXPECT_EQ(ToHexString(uint8_t(132u)), "84");
		EXPECT_EQ(ToHexString(uint8_t(15u)), "0f");
		EXPECT_EQ(ToHexString(112'345.f), "806cdb47");
		EXPECT_EQ(ToHexString('a'), "61");
	}
}
