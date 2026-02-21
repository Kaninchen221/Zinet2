#pragma once

#include "Zinet/SoftwareRenderer/ZtSoftwareRenderer.hpp"

#include <gtest/gtest.h>

namespace zt::software_renderer::tests
{
	class SoftwareRendererTests : public ::testing::Test
	{
	protected:

		SoftwareRenderer renderer;

	};

	TEST_F(SoftwareRendererTests, Pass)
	{

	}
}
