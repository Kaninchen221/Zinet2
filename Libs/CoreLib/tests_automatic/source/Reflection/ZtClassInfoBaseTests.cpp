#pragma once

#include "Zinet/Core/Reflection/ZtClassInfoBase.hpp"

#include <gtest/gtest.h>

namespace zt::core::tests
{

	class ClassInfoBaseTests : public ::testing::Test
	{
	protected:

		ClassInfoBaseTests()
		{
		}

		~ClassInfoBaseTests() override
		{
		}

		void SetUp() override
		{
		}

		void TearDown() override
		{
		}

		class ClassInfoBaseDerived : public ClassInfoBase
		{
		public:
			virtual std::string_view getClassName() const { return "ClassInfoBaseDerived"; };
		};
	};

	TEST_F(ClassInfoBaseTests, PassTest)
	{

	}

}