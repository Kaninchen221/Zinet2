#pragma once

#include "Zinet/Core/ZtArchive.hpp"
#include "Zinet/Core/ZtFile.hpp"
#include "Zinet/Core/ZtPaths.hpp"

#include <gtest/gtest.h>

namespace zt::core::tests
{
	struct SmallStruct
	{
		inline static const int ExpectedI = 1321312123;
		inline static const double ExpectedD = 123123.1223;

		int64_t i = ExpectedI;
		double d = ExpectedD;

        void operator << (Archive& archive)
        {
            archive << i;
            archive << d;
        }

        void serialize(JsonArchive& archive)
        {
			archive.serialize("i", i);
			archive.serialize("d", d);
		}

		void deserialize(JsonArchive& archive)
		{
			archive.deserialize("i", i);
			archive.deserialize("d", d);
		}

        void testValues()
		{
			ASSERT_EQ(i, SmallStruct::ExpectedI);
			ASSERT_EQ(d, SmallStruct::ExpectedD);
        }

        void changeValues()
        {
            i = 654345;
            d = 54323.213123;
        }
	};

    class ArchiveTests : public ::testing::Test
    {
    protected:

		std::filesystem::path testFolderPath = Paths::CurrentProjectRootPath() / "file_test_folder";

        static_assert(!std::is_default_constructible_v<Archive>);
    };

    TEST_F(ArchiveTests, DummyTest)
	{
		Archive::BufferT buffer;
		Archive archive{ &buffer };

		int i = 200;
        archive << i;
	}

	TEST_F(ArchiveTests, InputOutputTest)
	{
		Archive::BufferT buffer;
        int expectedInt = 201;

		SmallStruct smallStruct;

        float expectedFloat = 21312.123f;
        {
            OutputArchive outputArchive{ &buffer };
            outputArchive << expectedInt;
            outputArchive << expectedFloat;
            outputArchive << smallStruct;

            ASSERT_EQ(buffer.size(), sizeof(int) + sizeof(float) + sizeof(SmallStruct));
        }

        smallStruct.changeValues();

        {
            InputArchive inputArchive{ &buffer };
            int actualInt = 0;
            inputArchive << actualInt;
            ASSERT_EQ(actualInt, expectedInt);

            float actualFloat = 0;
            inputArchive << actualFloat;
            ASSERT_EQ(actualFloat, expectedFloat);

            inputArchive << smallStruct;
            smallStruct.testValues();
        }
	}

	TEST_F(ArchiveTests, FileTest)
	{
		Archive::BufferT buffer;
        const auto filePath = testFolderPath / "archiveTest.temp";

        SmallStruct smallStruct;
        {
            OutputArchive archive{ &buffer };
            archive << smallStruct;

            File file;
            file.open(filePath, FileOpenMode::Write, true);
            file.writeData(buffer);
		}

		smallStruct.changeValues();

        {
            File file;
            file.open(filePath, FileOpenMode::Read, true);
            buffer = file.readData();

            InputArchive archive{ &buffer };
            archive << smallStruct;
            smallStruct.testValues();
        }
	}

	TEST_F(ArchiveTests, JsonDummyTest)
	{
        JsonArchive::BufferT json;
		const auto filePath = testFolderPath / "archiveTest.temp";

        JsonArchive archive{ &json };
        const int dummyInt{};
        archive.serialize("int", dummyInt);
	}

	TEST_F(ArchiveTests, JsonTest)
	{
		JsonArchive::BufferT json;
		const auto filePath = testFolderPath / "archiveTest.temp";

        SmallStruct smallStruct;
        const int expectedInt = 2134;
        { // Serialize
            JsonArchive archive{ &json };
            archive.serialize("int", expectedInt);
            archive.serialize("smallStruct", smallStruct);
		}

		smallStruct.changeValues();

        { // Deserialize
            JsonArchive archive{ &json };
            int actualInt = {};
			archive.deserialize("int", actualInt);
			archive.deserialize("smallStruct", smallStruct);

            ASSERT_EQ(expectedInt, actualInt);
            smallStruct.testValues();
        }
	}
}