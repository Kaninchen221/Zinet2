#pragma once

#include "Zinet/Gameplay/Systems/ZtSystem.hpp"

#include "Zinet/Core/ZtObjectsStorage.hpp"

#include <gtest/gtest.h>

namespace zt::gameplay::tests
{
	class SystemTests : public ::testing::Test
	{
	protected:

		void SetUp() override
		{
			ASSERT_TRUE(system.init());
		}

		void TearDown() override
		{
			system.deinit();
		}

		core::ObjectsStorage objectsStorage;
		System system;

		static_assert(std::is_default_constructible_v<System>);
	};

	TEST_F(SystemTests, NodesTest)
	{
		ObjectHandle<Node> node = objectsStorage.createObject<Node>("node");
		system.addNode(node.createWeakHandle());
		ASSERT_EQ(node.getRefCount(), 1);

		ASSERT_EQ(system.getNodes().size(), 1);
	}
}