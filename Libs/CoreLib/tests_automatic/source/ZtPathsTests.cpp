#pragma once

#include <gtest/gtest.h>
#include "Zinet/Core/ZtPaths.hpp"

namespace zt::core::tests
{

	TEST(PathsTests, RootPathTest)
	{
		const auto path = Paths::RootPath();
		const auto recipeRootPath = path / "recipe_root.py";
		const auto licensePath = path / "LICENSE";

		EXPECT_TRUE(std::filesystem::exists(recipeRootPath) ||
					std::filesystem::exists(licensePath));
	}

	TEST(PathsTests, CurrentProjectRootPathTest)
	{
		const auto path = Paths::CurrentProjectRootPath();
		ASSERT_FALSE(path.empty());
	}

}