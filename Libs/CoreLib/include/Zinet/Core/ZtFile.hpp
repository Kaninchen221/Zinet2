#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"
#include "Zinet/Core/ZtFileFinder.hpp"
#include "Zinet/Core/ZtFileOpenMode.hpp"
#include "Zinet/Core/ZtObject.hpp"

#include <fstream>

namespace zt::core
{
	ZT_REFLECT_CLASS(NO_CONSTRUCTORS, NO_DESTRUCTOR, NO_OPERATORS)
	class ZINET_CORE_API File : public Object
	{
	protected:

		inline static core::ConsoleLogger Logger = core::ConsoleLogger::Create("File");

	public:

		File() = default;
		File(const File& other) = delete;
		File(File&& other) = default;

		File& operator = (const File& other) = delete;
		File& operator = (File&& other) = default;

		~File() noexcept;

		auto& get() const noexcept { return fileStream; }
		auto& get() noexcept { return fileStream; }

		void open(const std::filesystem::path& filePath, FileOpenMode openMode, bool binary = false);

		bool isOpen() const noexcept;

		bool isOkay() const noexcept;

		void log() const noexcept;

		std::string readLine();

		std::string readAll();

		std::vector<uint8_t> readData();

		void write(const std::string& string);

		static std::ios_base::openmode ToStdOpenMode(FileOpenMode openMode);

		void close();

		static bool RemoveFile(const std::filesystem::path& path);

		static File CreateFile(const std::filesystem::path& path);

	protected:

		ZT_REFLECT_MEMBER(ReadOnly)
		std::fstream fileStream;

	public:
/*GENERATED_CODE_START*/
		static_assert(IsObjectClassInherited); // Class using ZT_REFLECT_CLASS should inherit public from Object class
		const inline static bool RegisterClassResult = RegisterClass<File>();
		std::unique_ptr<ObjectBase> createCopy() const override { std::unique_ptr<ObjectBase> result = createCopyInternal<File>(); *result = *this; return result; }
		
		class ClassInfo : public zt::core::ClassInfoBase
		{
		public:
		
			std::string_view getClassName() const override { return "File"; }
		};
		const zt::core::ClassInfoBase* getClassInfo() const override { static ClassInfo classInfo; return &classInfo; }
		
		
		const decltype(fileStream)& getFileStream() const { return fileStream; }
		
/*GENERATED_CODE_END*/
	};

}