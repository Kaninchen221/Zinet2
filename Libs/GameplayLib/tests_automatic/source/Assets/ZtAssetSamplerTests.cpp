#pragma once

#include <Zinet/Core/Assets/ZtAssetsStorage.hpp>

#include <Zinet/Gameplay/Assets/ZtAssetSampler.hpp>
#include "Zinet/Gameplay/Systems/ZtSystemRenderer.hpp"
#include "Zinet/Gameplay/Systems/ZtSystemWindow.hpp"

#include <gtest/gtest.h>

namespace zt::gameplay::asset::tests
{
	class SamplerTests : public ::testing::Test
	{
	protected:

		void SetUp() override
		{
		}

		void TearDown() override
		{
		}

	};

	TEST_F(SamplerTests, Test)
	{

	}
}