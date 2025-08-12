#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"
#include "Zinet/Core/ZtFileFinder.hpp"
#include "Zinet/Core/ZtFileOpenMode.hpp"
#include "Zinet/Core/ZtObject.hpp"

#include <fstream>

namespace zt::core
{
	class File
	{
	protected:

		inline static core::ConsoleLogger Logger = core::ConsoleLogger::Create("File");

	public:

		using Byte = std::byte;

		ZINET_CORE_API File() = default;
		ZINET_CORE_API File(const File& other) = delete;
		ZINET_CORE_API File(File&& other) noexcept = default;

		ZINET_CORE_API File& operator = (const File& other) = delete;
		ZINET_CORE_API File& operator = (File&& other) noexcept = default;

		ZINET_CORE_API ~File();

		auto& get() const noexcept { return fileStream; }
		auto& get() noexcept { return fileStream; }

		ZINET_CORE_API void open(const std::filesystem::path& filePath, FileOpenMode openMode, bool binary = false);

		ZINET_CORE_API bool isOpen() const noexcept;

		ZINET_CORE_API bool isOkay() const noexcept;

		ZINET_CORE_API void log() const;

		ZINET_CORE_API std::string readLine();

		ZINET_CORE_API std::string readAll();

		ZINET_CORE_API std::vector<Byte> readData();

		ZINET_CORE_API void write(const std::string& string);

		ZINET_CORE_API void writeData(const std::vector<Byte>& bytes);

		ZINET_CORE_API static std::ios_base::openmode ToStdOpenMode(FileOpenMode openMode) noexcept;

		ZINET_CORE_API void close();

		ZINET_CORE_API static bool RemoveFile(const std::filesystem::path& path);

		ZINET_CORE_API static File CreateFile(const std::filesystem::path& path);

	protected:

		std::fstream fileStream;

	};

}