#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"
#include "Zinet/Core/ZtFileFinder.hpp"
#include "Zinet/Core/ZtFileOpenMode.hpp"
#include "Zinet/Core/ZtObject.hpp"

#include <fstream>

namespace zt::core
{

	ZT_REFLECT_CLASS(NO_CONSTRUCTORS, NO_OPERATORS)
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

		void open(const std::filesystem::path& filePath, FileOpenMode openMode);

		bool isOpen() const;

		std::string readLine();

		std::string readAll();

		void write(const std::string& string);

		static std::ios_base::openmode ToStdOpenMode(FileOpenMode openMode);

		void close();

		static bool RemoveFile(const std::filesystem::path& path);

		static File CreateFile(const std::filesystem::path& path);

	protected:

		std::fstream fileStream;

	public:
/*GENERATED_CODE_START*/
		static_assert(IsObjectClassInherited); // Class using ZT_REFLECT_CLASS should inherit public from Object class
		
		class ClassInfo : zt::core::ClassInfoBase
		{
		public:
		
			static std::string_view GetClassName() { return "File"; }
		};
		
/*GENERATED_CODE_END*/
	};

}