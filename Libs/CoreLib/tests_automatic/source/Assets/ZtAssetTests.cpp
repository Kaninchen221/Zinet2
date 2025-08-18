#pragma once

#include "Zinet/Core/Assets/ZtAsset.hpp"
#include "Zinet/Core/Assets/ZtAssetsStorage.hpp"

#include <gtest/gtest.h>

namespace zt::core::tests
{

	class AssetTests : public ::testing::Test
	{
	protected:

		Asset asset;
	};

	TEST_F(AssetTests, Pass)
	{
	}
}