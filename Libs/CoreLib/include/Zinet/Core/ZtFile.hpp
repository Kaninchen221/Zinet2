#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"
#include "Zinet/Core/ZtFileFinder.hpp"
#include "Zinet/Core/ZtFileOpenMode.hpp"
#include "Zinet/Core/ZtObject.hpp"

#include <fstream>

namespace zt::core
{
	class ZINET_CORE_API File : public Object
	{
	protected:

		inline static core::ConsoleLogger Logger = core::ConsoleLogger::Create("File");

	public:

		using Byte = std::byte;

		File() ZINET_API_POST = default;
		File(const File& other) ZINET_API_POST = delete;
		File(File&& other) ZINET_API_POST = default;

		File& operator = (const File& other) ZINET_API_POST = delete;
		File& operator = (File&& other) ZINET_API_POST = default;

		~File() ZINET_API_POST;

		auto& get() const ZINET_API_POST { return fileStream; }
		auto& get() ZINET_API_POST { return fileStream; }

		void open(const std::filesystem::path& filePath, FileOpenMode openMode, bool binary = false) ZINET_API_POST;

		bool isOpen() const ZINET_API_POST;

		bool isOkay() const ZINET_API_POST;

		void log() const ZINET_API_POST;

		std::string readLine() ZINET_API_POST;

		std::string readAll() ZINET_API_POST;

		std::vector<Byte> readData() ZINET_API_POST;

		void write(const std::string& string) ZINET_API_POST;

		void writeData(const std::vector<Byte>& data) ZINET_API_POST;

		static std::ios_base::openmode ToStdOpenMode(FileOpenMode openMode) ZINET_API_POST;

		void close() ZINET_API_POST;

		static bool RemoveFile(const std::filesystem::path& path) ZINET_API_POST;

		static File CreateFile(const std::filesystem::path& path) ZINET_API_POST;

	protected:

		std::fstream fileStream;

	};

}