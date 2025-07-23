#pragma once

#include "Zinet/Core/Assets/ZtAssetProperty.hpp"

#include <gtest/gtest.h>

namespace zt::core::assets::tests
{

	class AssetPropertyTests : public ::testing::Test
	{
	protected:

		AssetProperty<Asset> assetProperty{ "Property Name" };
	};

	TEST_F(AssetPropertyTests, Test)
	{
	}
}