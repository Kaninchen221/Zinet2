#pragma once

#include "Zinet/RHI/ZtRHIConfig.hpp"
#include "Zinet/RHI/ZtRHI.hpp"

#include <gtest/gtest.h>

namespace zt::rhi::tests
{

	class RHITests : public ::testing::Test
	{
	protected:

		RHITests()
		{
		}

		~RHITests() override
		{
		}

		void SetUp() override
		{
		}

		void TearDown() override
		{
		}

		RHI rhi;
	};

	//TEST_F(RHITests, InitializationTest)
	//{
	//	rhi.init();
	//
	//	rhi.deinit();
	//}

}