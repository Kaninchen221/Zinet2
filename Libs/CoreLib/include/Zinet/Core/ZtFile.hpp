#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"
#include "Zinet/Core/ZtFileFinder.hpp"
#include "Zinet/Core/ZtFileOpenMode.hpp"
#include "Zinet/Core/Reflection/ZtReflection.hpp"

#include <fstream>

namespace zt::core
{

	ZT_REFLECT_CLASS(NO_CONSTRUCTORS, NO_OPERATORS)
	class ZINET_CORE_API File
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

		static std::ios_base::openmode ToStdOpenMode(FileOpenMode openMode);

		void close();

		static bool RemoveFile(const std::filesystem::path& path);

		static File CreateFile(const std::filesystem::path& path);

	protected:

		std::fstream fileStream;

	public:
/*GENERATED_CODE_START*/ 
		
		class ClassInfo
		{
		public:
		
			static std::string_view GetClassName() { return "File"; }
		};
/*GENERATED_CODE_END*/
	};

}