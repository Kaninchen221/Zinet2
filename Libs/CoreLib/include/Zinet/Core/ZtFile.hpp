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

		File() = default;
		File(const File& other) = delete;
		File(File&& other) = default;

		File& operator = (const File& other) = delete;
		File& operator = (File&& other) = default;

		~File() ZINET_API_POST;

		auto& get() const ZINET_API_POST { return fileStream; }
		auto& get() ZINET_API_POST { return fileStream; }

		void open(const std::filesystem::path& filePath, FileOpenMode openMode, bool binary = false);

		bool isOpen() const ZINET_API_POST;

		bool isOkay() const ZINET_API_POST;

		void log() const ZINET_API_POST;

		std::string readLine();

		std::string readAll();

		std::vector<uint8_t> readData();

		void write(const std::string& string);

		static std::ios_base::openmode ToStdOpenMode(FileOpenMode openMode);

		void close();

		static bool RemoveFile(const std::filesystem::path& path);

		static File CreateFile(const std::filesystem::path& path);

	protected:

		std::fstream fileStream;

	};

}