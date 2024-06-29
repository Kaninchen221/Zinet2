#include "Zinet/Core/ZtFile.hpp"

namespace zt::core
{

	File::~File() noexcept
	{
		if (isOpen())
		{
			close();
		}
	}

	void File::open(const FileFinder::Path& filePath, FileOpenMode openMode)
	{
		std::ios_base::openmode stdOpenMode = ToStdOpenMode(openMode);
		fileStream.open(filePath, stdOpenMode);
	}

	bool File::isOpen() const
	{
		return fileStream.is_open();
	}

	std::string File::readLine()
	{
		std::string line;
		std::getline(fileStream, line);
		return line;
	}

	std::string File::readAll()
	{
		std::string line;
		std::getline(fileStream, line, '\0');
		return line;
	}

	std::ios_base::openmode File::ToStdOpenMode(FileOpenMode openMode)
	{
		switch (openMode)
		{
		case FileOpenMode::App:
			return std::ios_base::app;

		case FileOpenMode::Binary:
			return std::ios_base::binary;

		case FileOpenMode::In:
			return std::ios_base::in;

		case FileOpenMode::Out:
			return std::ios_base::out;

		case FileOpenMode::Truncate:
			return std::ios_base::trunc;

		case FileOpenMode::Ate:
			return std::ios_base::ate;
		}

		return std::ios_base::in;
	}

	void File::close()
	{
		fileStream.close();
	}

}