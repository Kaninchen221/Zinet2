#pragma once

#include <gtest/gtest.h>
#include "Zinet/Core/ZtFileFinder.hpp"

namespace zt::core::tests
{

	class FileFinderTests : public ::testing::Test
	{
	protected:

	};

	TEST_F(FileFinderTests, CurrentPathTest)
	{
		FileFinder::Path path = FileFinder::CurrentPath();
		ASSERT_FALSE(path.empty());
	}

	TEST_F(FileFinderTests, CurrentProjectRootPathTest)
	{
		FileFinder::Path path = FileFinder::CurrentProjectRootPath();
		ASSERT_FALSE(path.empty());
	}

	TEST_F(FileFinderTests, FindFilesInSpecificFolderTest)
	{
		FileFinder::Path folderName = "file_finder_test_folder";
		FileFinder::Path path = FileFinder::CurrentProjectRootPath();
		path /= folderName;
		FileFinder::FolderInfo files = FileFinder::FindFiles(path);

		size_t expectedFilesCount = 3u;
		size_t actualFilesCount = files.size();
		ASSERT_EQ(actualFilesCount, expectedFilesCount);
	}

	TEST_F(FileFinderTests, FindFilesInSpecificFolderWithSpecificExtensionTest)
	{
		FileFinder::Path folderName = "file_finder_test_folder";
		FileFinder::Path path = FileFinder::CurrentProjectRootPath();
		path /= folderName;
		FileFinder::Extension extension = ".txt2";

		FileFinder::FolderInfo files = FileFinder::FindFiles(path, extension);
		size_t expectedFilesCount = 2u;
		size_t actualFilesCount = files.size();
		ASSERT_EQ(actualFilesCount, expectedFilesCount);
	}

}