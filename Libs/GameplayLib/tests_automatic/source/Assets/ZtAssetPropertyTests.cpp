#pragma once

#include "Zinet/Gameplay/Assets/ZtAssetProperty.hpp"

#include <gtest/gtest.h>

namespace zt::gameplay::tests
{

	class AssetPropertyTests : public ::testing::Test
	{
	protected:

		AssetProperty<core::Asset> assetProperty{ "Property Name" };
	};

	TEST_F(AssetPropertyTests, Test)
	{
	}
}