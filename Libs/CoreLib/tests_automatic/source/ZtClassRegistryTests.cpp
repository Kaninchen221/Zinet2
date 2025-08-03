#pragma once

#include "Zinet/Core/ZtClassRegistry.hpp"

#include <gtest/gtest.h>

namespace zt::core::tests
{

	class ClassRegistryTests : public ::testing::Test
	{
	protected:

		using ClassRegistry = ClassRegistry<Object>;
		ClassRegistry classRegistry;
	};

	class ClassType : public Object
	{ 

	};

	TEST_F(ClassRegistryTests, Pass)
	{
		classRegistry.registerClass<ClassType>({ "ClassType0", "type0" });
		classRegistry.registerClass<ClassType>({ "ClassType1", "type1" });
		ASSERT_FALSE(classRegistry.getClasses().empty());

		auto cdo = classRegistry.getClassByName("type1");
		ASSERT_TRUE(cdo);
	}
}