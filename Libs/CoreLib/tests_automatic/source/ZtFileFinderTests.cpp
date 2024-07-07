#pragma once

#include <gtest/gtest.h>

#include "Zinet/Core/ZtFileFinder.hpp"
#include "Zinet/Core/ZtPaths.hpp"

namespace zt::core::tests
{

	class FileFinderTests : public ::testing::Test
	{
	protected:

	};

	TEST_F(FileFinderTests, FindFilesInSpecificFolderTest)
	{
		auto test = [&](const bool recursive, const size_t expectedFilesCount)
		{
			const std::filesystem::path folderName = "file_finder_test_folder";
			auto path = Paths::CurrentProjectRootPath();
			path /= folderName;
			auto files = FileFinder::FindFiles(path, recursive);

			size_t actualFilesCount = files.size();
			EXPECT_EQ(actualFilesCount, expectedFilesCount);
		};

		test(false, 3u);
		test(true, 5u);
	}

	TEST_F(FileFinderTests, FindFilesInSpecificFolderWithSpecificExtensionTest)
	{
		auto test = [&](const bool recursive, const size_t expectedFilesCount)
		{
			const std::filesystem::path folderName = "file_finder_test_folder";
			auto path = Paths::CurrentProjectRootPath();
			path /= folderName;
			const std::filesystem::path extension = ".txt2";

			auto files = FileFinder::FindFiles(path, extension, recursive);
			size_t actualFilesCount = files.size();
			EXPECT_EQ(actualFilesCount, expectedFilesCount);
		};

		test(false, 2u);
		test(true, 3u);
	}

}