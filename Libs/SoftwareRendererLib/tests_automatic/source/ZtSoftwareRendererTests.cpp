#pragma once

#include "Zinet/SoftwareRenderer/ZtSoftwareRendererConfig.hpp"
#include <gtest/gtest.h>

namespace zt::software_renderer::tests
{

	class SoftwareRendererTests : public ::testing::Test
	{
	protected:

		SoftwareRendererTests()
		{
		}

		~SoftwareRendererTests() override
		{
		}

		void SetUp() override
		{
		}

		void TearDown() override
		{
		}

	};

	TEST_F(SoftwareRendererTests, Pass)
	{
		
	}

}