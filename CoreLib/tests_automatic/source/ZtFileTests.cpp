#pragma once

#include <gtest/gtest.h>

#include "Zinet/Core/ZtFile.hpp"

namespace zt::core::tests
{

	class FileTests : public ::testing::Test
	{
	protected:

		File file;

		FileFinder::Path testFolderPath = FileFinder::CurrentProjectRootPath() / "file_test_folder";
		FileFinder::Path pathToReadOnlyFile = testFolderPath / "read_only_file.txt";
	};

	TEST_F(FileTests, OpenTest)
	{
		bool isOpen = file.isOpen();
		ASSERT_FALSE(isOpen);

		file.open(pathToReadOnlyFile, FileOpenMode::In);
		isOpen = file.isOpen();
		ASSERT_TRUE(isOpen);
	}

	TEST_F(FileTests, ReadLineTest)
	{
		file.open(pathToReadOnlyFile, FileOpenMode::In);

		std::string actualFirstLine = file.readLine();
		std::string expectedFirstLine = "test text";
		ASSERT_EQ(actualFirstLine, expectedFirstLine);

		std::string actualSecondLine = file.readLine();
		std::string expectedSecondLine = "should be never modified";
		ASSERT_EQ(actualSecondLine, expectedSecondLine);
	}

	TEST_F(FileTests, ZtFileOpenModeToStdOpenMode)
	{
		std::ios_base::openmode actualOpenMode = File::ToStdOpenMode(FileOpenMode::Truncate);
		std::ios_base::openmode expectedOpenMode = std::ios_base::trunc;
		ASSERT_EQ(actualOpenMode, expectedOpenMode);
	}

	TEST_F(FileTests, ReadAllTest)
	{
		file.open(pathToReadOnlyFile, FileOpenMode::In);
		std::string actualText = file.readAll();
		std::string expectedText = "test text\nshould be never modified\n";
		ASSERT_EQ(actualText, expectedText);
	}

}