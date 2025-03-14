#pragma once

#include "Zinet/GameplayLib/ZtNode.hpp"

#include <gtest/gtest.h>

namespace zt::gameplay_lib::tests
{

	class NodeTests : public ::testing::Test
	{
	protected:

		NodeTests()
		{
		}

		~NodeTests() override {
		}

		void SetUp() override 
		{
		}

		void TearDown() override {
		}

		Node node;
	};

	TEST_F(NodeTests, Test)
	{
		
	}
}