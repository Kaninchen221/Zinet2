#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"
#include "Zinet/Core/ZtFileFinder.hpp"
#include "Zinet/Core/ZtFileOpenMode.hpp"

#include <fstream>

namespace zt::core
{

	ZT_REFLECT_CLASS()
	class ZINET_CORE_API File
	{

	public:

		File() = default;
		File(const File& other) = delete;
		File(File&& other) = default;

		File& operator = (const File& other) = delete;
		File& operator = (File&& other) = default;

		~File() noexcept;

		void open(const FileFinder::Path& filePath, FileOpenMode openMode);

		bool isOpen() const;

		std::string readLine();

		std::string readAll();

		static std::ios_base::openmode ToStdOpenMode(FileOpenMode openMode);

		void close();

	protected:

		std::fstream fileStream;
	};

}