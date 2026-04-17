#pragma once

#include "Zinet/ComputerSystems/ZtMemoryUtils.hpp"

#include "Zinet/Core/ZtLogger.hpp"

#include <gtest/gtest.h>

#include <array>
#include <limits>

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
		// uint8_t
		EXPECT_EQ(ToHexString(uint8_t(255u)), "ff");
		EXPECT_EQ(ToHexString(uint8_t(0u)), "00");
		EXPECT_EQ(ToHexString(uint8_t(1u)), "01");
		EXPECT_EQ(ToHexString(uint8_t(132u)), "84");
		EXPECT_EQ(ToHexString(uint8_t(15u)), "0f");

		// uint16_t
		const uint16_t twoBytesMax = std::numeric_limits<uint16_t>::max();
		EXPECT_EQ(ToHexString(twoBytesMax), "ffff");

		const uint16_t twoBytesHalf = 255;
		EXPECT_EQ(ToHexString(twoBytesHalf), "00ff");

		// Float
		EXPECT_EQ(ToHexString(112'345.f), "47db6c80");

		// Char
		EXPECT_EQ(ToHexString('a'), "61");

		Logger->info("20 decimal as hex: {}", ToHexString(20));
		Logger->info("20 ^ 20 = {}", ToHexString(20 ^ 20)); // a ^ a = 0
		Logger->info("0 ^ 20 = {}", ToHexString(0 ^ 20)); // 0 ^ a = a
	}

	TEST_F(MemoryUtilsTests, InPlaceSwapTest)
	{
		int a = 100;
		int b = 45666;

		InPlaceSwap(a, b);

		EXPECT_EQ(a, 45666);
		EXPECT_EQ(b, 100);
	}

	TEST_F(MemoryUtilsTests, ReverseArrayTest)
	{
		std::array<int, 7> arrayIntEvenLength = { 0, 1, 2, 3, 4, 5, 6 };
		const std::array<int, 7> arrayIntEvenLengthExpected = { 6, 5, 4, 3, 2, 1, 0 };
		ReverseArray(arrayIntEvenLength);
		EXPECT_EQ(arrayIntEvenLength, arrayIntEvenLengthExpected);

		std::array<int, 3> arrayIntOddLength = { 0, 1, 2 };
		const std::array<int, 3> arrayIntOddLengthExpected = { 2, 1, 0 };
		ReverseArray(arrayIntOddLength);
		EXPECT_EQ(arrayIntOddLength, arrayIntOddLengthExpected);
	}

	TEST_F(MemoryUtilsTests, GetOnlyLeastSignificantByteTest)
	{
		const uint32_t value = 0x87654321;
		const uint32_t result = GetOnlyLeastSignificantByte(value);
		const uint32_t expected = 0x00000021;
		EXPECT_EQ(result, expected);

		Logger->info("Only Least Significant Byte of '0x87654321': {}", ToHexString(result));
	}

	TEST_F(MemoryUtilsTests, ComplementBytesLeastByteUnchangedTest)
	{
		const uint32_t value = 0x87654321;
		const uint32_t result = ComplementBytesLeastByteUnchanged(value);
		const uint32_t expected = 0x789ABC21;
		EXPECT_EQ(result, expected);

		Logger->info("Result: {}", ToHexString(result));
	}

	TEST_F(MemoryUtilsTests, BisTest)
	{
		const uint8_t value = 0b00110011;
		const uint8_t mask  = 0b00001111;
		const uint8_t result = Bis(value, mask);
		const uint8_t expected = 0b00111111;
		EXPECT_EQ(result, expected);

		Logger->info("Result: {}", ToHexString(result));
	}

	TEST_F(MemoryUtilsTests, BicTest)
	{
		const uint8_t value = 0b00110011;
		const uint8_t mask  = 0b00001111;
		const uint8_t result = Bic(value, mask);
		const uint8_t expected = 0b00110000;
		EXPECT_EQ(result, expected);

		Logger->info("Result: {}", ToHexString(result));
	}

	TEST_F(MemoryUtilsTests, BoolOrTest)
	{

	}
}
