#pragma once

#include "Zinet/RHI/ZtVertex.hpp"
#include "Zinet/RHI/Assets/ZtShaderSource.hpp"

#include <gtest/gtest.h>

#include <type_traits>

namespace zt::rhi::assets::tests
{

	class ShaderSourceTests : public ::testing::Test
	{
	protected:

		ShaderSourceTests()
		{
		}

		~ShaderSourceTests() override
		{
		}

		void SetUp() override
		{
		}

		void TearDown() override
		{
		}

		static_assert(std::is_base_of_v<core::assets::Asset, ShaderSource>);
	};

	TEST_F(ShaderSourceTests, Pass)
	{

	}

}