#pragma once

#include "Zinet/Core/ZtConfigVariable.hpp"
#include <Zinet/Core/ZtArchive.hpp>

#include <gtest/gtest.h>

namespace zt::core::tests
{

	class ConfigVariableTests : public ::testing::Test
	{
	protected:

		ConfigVariableTests()
		{
		}

		~ConfigVariableTests() override
		{
		}

		void SetUp() override
		{
		}

		void TearDown() override
		{
		}

		static_assert(!std::is_default_constructible_v<ConfigVariable<int>>);
		static_assert(std::is_constructible_v<ConfigVariable<int>, std::string, int, std::string>);
	};

	TEST_F(ConfigVariableTests, GettersTest)
	{
		const auto expectedName = "Int32";
		const auto expectedValue = 32;
		const auto expectedToolTip = "ToolTip";

		CVar<int32_t> cvarInt32( expectedName, 32, expectedToolTip );

		EXPECT_EQ(expectedName, cvarInt32.getDisplayName());
		EXPECT_EQ(expectedValue, cvarInt32.getValue());
		EXPECT_EQ(expectedToolTip, cvarInt32.getToolTip());
	}

	TEST_F(ConfigVariableTests, TypesTest)
	{
		CVar<float> cvarFloat("float", 32.213f, "ToolTip");
		CVar<double> cvarDouble("double", 32.213, "ToolTip");
		CVar<std::string> cvarStdString("std::string", "string", "ToolTip");
	}

	TEST_F(ConfigVariableTests, SerializeDeserializeTest)
	{
		const auto name = "float";
		const float expectedValue = 32.f;
		const auto toolTip = "ToolTip";

		Json json;
		JsonArchive archive{ &json };
		{
			CVar<float> cvar(name, 32, toolTip);
			cvar.serialize(archive);
		}

		{
			CVar<float> cvar(name, 0, toolTip);
			cvar.deserialize(archive);

			EXPECT_EQ(expectedValue, cvar.getValue());
		}
	}
}