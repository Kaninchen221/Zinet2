#pragma once

#include <gtest/gtest.h>

#include "Zinet/VulkanRenderer/ZtConcepts.hpp"

namespace zt::vulkan_renderer::tests
{
	class ConceptsTests : public ::testing::Test
	{
	protected:

		void SetUp() override
		{
		}

		void TearDown() override
		{
		}
	};

	TEST_F(ConceptsTests, IsCommandsTTest)
	{
		auto validCommands = [](const CommandBuffer&) -> void
		{};

		ASSERT_TRUE(IsCommandsT<decltype(validCommands)>);

		auto invalidCommands = [](int) -> void
		{};

		ASSERT_FALSE(IsCommandsT<decltype(invalidCommands)>);
	}
}