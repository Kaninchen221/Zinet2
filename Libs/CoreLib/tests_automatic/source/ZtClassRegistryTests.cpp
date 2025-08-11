#pragma once

#include "Zinet/Core/ZtClassRegistry.hpp"

#include <gtest/gtest.h>

namespace zt::core::tests
{

	class ClassRegistryTests : public ::testing::Test
	{
	protected:

		ClassRegistry<Object> classRegistry;
	};

	class ClassType : public Object
	{
	public:
		virtual std::string getClassName() const { return "zt::core::tests::ClassType"; }
	};

	TEST_F(ClassRegistryTests, Pass)
	{
		classRegistry.registerClass<ClassType>();
		ASSERT_FALSE(classRegistry.getCDOs().empty());

		auto cdo = classRegistry.getClassByName("zt::core::tests::ClassType");
		ASSERT_TRUE(cdo);
	}
}