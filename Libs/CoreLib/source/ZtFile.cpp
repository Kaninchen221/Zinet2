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

	void File::open(const std::filesystem::path& filePath, FileOpenMode openMode)
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
		fileStream.seekg(0);
		std::string line;
		std::getline(fileStream, line, '\0');
		return line;
	}

	void File::write(const std::string& string)
	{
		fileStream << string;
	}

	std::ios_base::openmode File::ToStdOpenMode(FileOpenMode openMode)
	{
		switch (openMode)
		{
			case FileOpenMode::Read:
				return std::ios_base::in;

			case FileOpenMode::Write:
				return std::ios_base::out;

			case FileOpenMode::ReadWrite:
				return std::ios_base::in | std::ios_base::out;

			default:
				Logger->error("Invalid openMode: {}", openMode);
				return std::ios_base::out;
		}
	}

	void File::close()
	{
		fileStream.close();
	}

	bool File::RemoveFile(const std::filesystem::path& path)
	{
		if (!std::filesystem::is_regular_file(path))
		{
			Logger->error("Path is not leading to a file. Path: {}", path.string());
			return false;
		}

		std::error_code errorCode;
		std::filesystem::remove(path, errorCode);
		if (errorCode)
		{
			Logger->error("Internal remove returned error code: {}", errorCode.value());
			return false;
		}

		return true;
	}

	File File::CreateFile(const std::filesystem::path& path)
	{
		if (std::filesystem::exists(path))
		{
			Logger->error("File already exists, path: {}", path.string());
			return File{};
		}

		File file;
		file.open(path, FileOpenMode::Write);
		return file;
	}

}