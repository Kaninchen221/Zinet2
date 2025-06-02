#pragma once

#include <gtest/gtest.h>

#include "Zinet/Core/ZtImage.hpp"
#include "Zinet/Core/ZtPaths.hpp"

namespace zt::core::tests
{

	class ImageTests : public ::testing::Test
	{
	protected:

		Image image;

		const std::filesystem::path testFolderPath = Paths::CurrentProjectRootPath() / "test_files";
		const std::filesystem::path imagePath = testFolderPath / "image.png";
	};

	TEST_F(ImageTests, Test)
	{
		const std::int32_t expectedComponents = 4;
		ASSERT_TRUE(image.loadFromFile(imagePath, expectedComponents));
		ASSERT_TRUE(image.data());
		ASSERT_EQ(image.getWidth(), 32);
		ASSERT_EQ(image.getHeight(), 32);
		ASSERT_EQ(image.getComponents(), expectedComponents);
		ASSERT_EQ(image.getSize(), 4096);

		image.destroy();
		ASSERT_FALSE(image.data());
		ASSERT_EQ(image.getWidth(), 0);
		ASSERT_EQ(image.getHeight(), 0);
		ASSERT_EQ(image.getComponents(), 0);
		ASSERT_EQ(image.getSize(), 0);
	}
}