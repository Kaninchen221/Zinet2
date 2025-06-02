#pragma once

#include <gtest/gtest.h>

#include "Zinet/Core/ZtImage.hpp"
#include "Zinet/Core/ZtPaths.hpp"

namespace zt::core::tests
{
	void TestIsImageReallyInvalid(const Image& image)
	{
		ASSERT_FALSE(image.data());
		ASSERT_EQ(image.getWidth(), 0);
		ASSERT_EQ(image.getHeight(), 0);
		ASSERT_EQ(image.getComponents(), 0);
		ASSERT_EQ(image.getSize(), 0);
	};

	class ImageTests : public ::testing::Test
	{
	protected:

		Image image;
		const std::int32_t expectedComponents = 4;

		const std::filesystem::path testFolderPath = Paths::CurrentProjectRootPath() / "test_files";
		const std::filesystem::path imagePath = testFolderPath / "image.png";

		void SetUp() override
		{
			ASSERT_TRUE(image.loadFromFile(imagePath, expectedComponents));
			ASSERT_TRUE(image.data());
			ASSERT_EQ(image.getWidth(), 32);
			ASSERT_EQ(image.getHeight(), 32);
			ASSERT_EQ(image.getComponents(), expectedComponents);
			ASSERT_EQ(image.getSize(), 4096);
		}

		void TearDown() override
		{
			image.destroy();
			TestIsImageReallyInvalid(image);
		}

		static_assert(std::is_base_of_v<Object, Image>);

		static_assert(std::is_default_constructible_v<Image>);
		static_assert(!std::is_copy_constructible_v<Image>);
		static_assert(!std::is_copy_assignable_v<Image>);
		static_assert(std::is_move_constructible_v<Image>);
		static_assert(std::is_move_assignable_v<Image>);
		static_assert(std::is_destructible_v<Image>);
	};

	TEST_F(ImageTests, PassTest)
	{}

	TEST_F(ImageTests, MoveTest)
	{
		auto moved = std::move(image);
		ASSERT_TRUE(moved.data());
		ASSERT_EQ(moved.getWidth(), 32);
		ASSERT_EQ(moved.getHeight(), 32);
		ASSERT_EQ(moved.getComponents(), expectedComponents);
		ASSERT_EQ(moved.getSize(), 4096);

		TestIsImageReallyInvalid(image);
	}
}