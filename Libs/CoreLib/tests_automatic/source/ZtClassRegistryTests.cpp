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

	};

	TEST_F(ClassRegistryTests, Pass)
	{
		classRegistry.registerClass<ClassType>("ClassType0");
		ASSERT_FALSE(classRegistry.getCDOs().empty());

		auto cdo = classRegistry.getClassByName("ClassType0");
		ASSERT_TRUE(cdo);
	}
}