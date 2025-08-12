#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"
#include "Zinet/Core/ZtFileFinder.hpp"
#include "Zinet/Core/ZtFileOpenMode.hpp"
#include "Zinet/Core/ZtObject.hpp"

#include <fstream>

namespace zt::core
{
	class ZINET_CORE_API File
	{
	protected:

		inline static core::ConsoleLogger Logger = core::ConsoleLogger::Create("File");

	public:

		using Byte = std::byte;

		File() = default;
		File(const File& other) = delete;
		File(File&& other) noexcept = default;

		File& operator = (const File& other) = delete;
		File& operator = (File&& other) noexcept = default;

		~File();

		auto& get() const noexcept { return data.fileStream; }
		auto& get() noexcept { return data.fileStream; }

		void open(const std::filesystem::path& filePath, FileOpenMode openMode, bool binary = false);

		bool isOpen() const noexcept;

		bool isOkay() const noexcept;

		void log() const;

		std::string readLine();

		std::string readAll();

		std::vector<Byte> readData();

		void write(const std::string& string);

		void writeData(const std::vector<Byte>& bytes);

		static std::ios_base::openmode ToStdOpenMode(FileOpenMode openMode) noexcept;

		void close();

		static bool RemoveFile(const std::filesystem::path& path);

		static File CreateFile(const std::filesystem::path& path);

	protected:

		struct {
			std::fstream fileStream;
		} data;

	};

}