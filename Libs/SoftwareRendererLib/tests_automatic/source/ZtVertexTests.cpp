#pragma once

#include "Zinet/Core/ZtLogger.hpp"

#include "Zinet/SoftwareRenderer/ZtVertex.hpp"

#include <gtest/gtest.h>

namespace zt::software_renderer::tests
{
	class VertexTests : public ::testing::Test
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("VertexTests");

	};

	TEST_F(VertexTests, Pass)
	{

	}
}
