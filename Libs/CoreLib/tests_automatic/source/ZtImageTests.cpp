#pragma once

#include <gtest/gtest.h>

#include "Zinet/Core/ZtImage.hpp"
#include "Zinet/Core/ZtPaths.hpp"
#include "Zinet/Core/ZtFile.hpp"

namespace zt::core::tests
{
	void TestIsImageReallyInvalid(const Image& image)
	{
		ASSERT_FALSE(image.getData());
		ASSERT_EQ(image.getWidth(), 0);
		ASSERT_EQ(image.getHeight(), 0);
		ASSERT_EQ(image.getComponents(), 0);
		ASSERT_EQ(image.getSize(), 0);
	};

	void TestImageIsValid(const Image& image)
	{
		ASSERT_TRUE(image.getData());
		ASSERT_EQ(image.getWidth(), 32);
		ASSERT_EQ(image.getHeight(), 32);
		ASSERT_EQ(image.getComponents(), 4);
		ASSERT_EQ(image.getSize(), 4096);
	}

	class ImageTests : public ::testing::Test
	{
	protected:

		Image image;

		const std::filesystem::path testFolderPath = Paths::CurrentProjectRootPath() / "test_files";
		const std::filesystem::path imagePath = testFolderPath / "image.png";

		void SetUp() override
		{
		}

		void TearDown() override
		{
			image.destroy();
			TestIsImageReallyInvalid(image);
		}

		void loadFromFile()
		{
			ASSERT_TRUE(image.loadFromFile(imagePath, 4)); 
			TestImageIsValid(image);
		}

		void loadFromData()
		{
			File file;
			file.open(imagePath, FileOpenMode::Read, true);
			ASSERT_TRUE(file.isOpen());
			ASSERT_TRUE(file.isOkay());
			auto data = file.readData();
			
			if (!file.isOkay())
			{
				file.log();
				FAIL() << "Something bad happened with the file";
			}

			ASSERT_TRUE(image.loadFromData(data, 4));
			TestImageIsValid(image);
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
		loadFromFile();

		auto moved = std::move(image);

		TestIsImageReallyInvalid(image);
	}

	TEST_F(ImageTests, LoadFromData)
	{
		loadFromData();
	}
}