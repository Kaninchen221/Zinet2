#pragma once

#include "Zinet/Core/ZtClassDefaultObjectRegistry.hpp"
#include "Zinet/Core/ZtObject.hpp"

#include <gtest/gtest.h>

namespace zt::core::tests
{

	class ClassDefaultObjectRegistryTests : public ::testing::Test
	{
	protected:

		ClassDefaultObjectRegistryTests()
		{
		}

		~ClassDefaultObjectRegistryTests() override
		{
		}

		void SetUp() override
		{
		}

		void TearDown() override
		{
		}

	};

}