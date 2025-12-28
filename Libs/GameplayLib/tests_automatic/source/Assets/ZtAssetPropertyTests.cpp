#pragma once

#include "Zinet/Gameplay/Assets/ZtAssetProperty.hpp"

#include "Zinet/Core/Assets/ZtAsset.hpp"

#include <gtest/gtest.h>

namespace zt::gameplay::asset::tests
{

	class AssetPropertyTests : public ::testing::Test
	{
	protected:

		Property<core::Asset> assetProperty{ "Property Name" };
	};

	TEST_F(AssetPropertyTests, Test)
	{
	}
}