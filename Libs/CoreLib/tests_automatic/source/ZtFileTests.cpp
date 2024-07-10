#pragma once

#include <gtest/gtest.h>

#include "Zinet/Core/ZtFile.hpp"
#include "Zinet/Core/ZtPaths.hpp"

namespace zt::core::tests
{

	class FileTests : public ::testing::Test
	{
	protected:

		File file;

		std::filesystem::path testFolderPath = Paths::CurrentProjectRootPath() / "file_test_folder";
		std::filesystem::path pathToReadOnlyFile = testFolderPath / "read_only_file.txt";
		std::filesystem::path pathToReadWriteFile = testFolderPath / "read_write_file.txt";
	};

	TEST_F(FileTests, OpenTest)
	{
		bool isOpen = file.isOpen();
		ASSERT_FALSE(isOpen);

		file.open(pathToReadOnlyFile, FileOpenMode::Read);
		isOpen = file.isOpen();
		ASSERT_TRUE(isOpen);
	}

	TEST_F(FileTests, ReadLineTest)
	{
		file.open(pathToReadOnlyFile, FileOpenMode::Read);

		std::string actualFirstLine = file.readLine();
		std::string expectedFirstLine = "test text";
		ASSERT_EQ(actualFirstLine, expectedFirstLine);

		std::string actualSecondLine = file.readLine();
		std::string expectedSecondLine = "should be never modified";
		ASSERT_EQ(actualSecondLine, expectedSecondLine);
	}

	TEST_F(FileTests, ReadAllTest)
	{
		file.open(pathToReadOnlyFile, FileOpenMode::Read);
		std::string actualText = file.readAll();
		std::string expectedText = "test text\nshould be never modified\n";
		ASSERT_EQ(actualText, expectedText);
	}

	TEST_F(FileTests, WriteTest)
	{
		file.open(pathToReadWriteFile, FileOpenMode::ReadWrite);

		const std::string expectedText = "read write file";
		file.write(expectedText);
		EXPECT_EQ(file.readAll(), expectedText);
	}

	TEST_F(FileTests, ZtFileOpenModeToStdOpenModeTest)
	{
		std::ios_base::openmode actualOpenMode = File::ToStdOpenMode(FileOpenMode::ReadWrite);
		std::ios_base::openmode expectedOpenMode = std::ios_base::in | std::ios_base::out;
		ASSERT_EQ(actualOpenMode, expectedOpenMode);
	}

	TEST_F(FileTests, CreateRemoveFileTest)
	{
		const std::filesystem::path testFilePath = testFolderPath / "createFileTest.txt";

		if (std::filesystem::exists(testFilePath))
			ASSERT_TRUE(File::RemoveFile(testFilePath));

		File createdFile = File::CreateFile(testFilePath);
		ASSERT_TRUE(createdFile.isOpen());
		createdFile.close();

		if (std::filesystem::exists(testFilePath))
			File::RemoveFile(testFilePath);
		else
			FAIL() << "File should exists after CreateFile";
	}
}