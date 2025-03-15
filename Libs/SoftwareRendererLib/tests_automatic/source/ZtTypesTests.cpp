#pragma once

#include "Zinet/SoftwareRenderer/ZtTypes.hpp"

#include "Zinet/Core/ZtLogger.hpp"

#include <gtest/gtest.h>

namespace zt::software_renderer::tests
{

	class TypesTests : public ::testing::Test
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("SoftwareRenderer TypesTests");

		TypesTests()
		{
		}

		~TypesTests() override
		{
		}

		void SetUp() override
		{
		}

		void TearDown() override
		{
		}

	};

	TEST_F(TypesTests, VertexTests)
	{
		constexpr const auto size = sizeof(Vertex);
		Logger->info("Vertex Size: {}", size);
	}

	TEST_F(TypesTests, TriangleTests)
	{
		constexpr const auto size = sizeof(Triangle);
		Logger->info("Triangle Size: {}", size);
	}

	TEST_F(TypesTests, PixelTests)
	{
		constexpr const auto size = sizeof(Pixel);
		Logger->info("Pixel Size: {}", size);
		Logger->info("Pixel Align: {}", alignof(Pixel));
		Logger->info("Pixel Coords Size: {}", sizeof(decltype(Pixel{}.coords)));
		Logger->info("Pixel Color Size: {}", sizeof(decltype(Pixel{}.color)));
		Logger->info("Pixel UV Size: {}", sizeof(decltype(Pixel{}.uv)));
	}

	TEST_F(TypesTests, Colors)
	{
		constexpr const auto size = sizeof(Color);
		constexpr const auto expectedSize = 4;
		static_assert(size == expectedSize);
		Logger->info("Color Size: {}", size);
		Logger->info("Address size of Color: {}", sizeof(Color*));
	}
}