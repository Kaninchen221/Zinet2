#pragma once

#include "Zinet/Core/Assets/ZtAsset.hpp"

#include <gtest/gtest.h>

namespace zt::core::assets::tests
{

	class AssetTests : public ::testing::Test
	{
	protected:

		AssetTests()
		{
		}

		~AssetTests() override
		{
		}

		void SetUp() override
		{
		}

		void TearDown() override
		{
		}

		Asset asset;
	};

	TEST_F(AssetTests, Test)
	{
	}

}