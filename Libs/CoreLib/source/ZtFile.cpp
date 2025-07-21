#include "Zinet/Core/ZtFile.hpp"

namespace zt::core
{

	File::~File() ZINET_API_POST
	{
		if (isOpen())
		{
			close();
		}
	}

	void File::open(const std::filesystem::path& filePath, FileOpenMode openMode, bool binary) ZINET_API_POST
	{
		std::ios_base::openmode stdOpenMode = ToStdOpenMode(openMode);
		if (binary)
		{
			stdOpenMode |= std::ios_base::binary;
		}

		fileStream.open(filePath, stdOpenMode);
	}

	bool File::isOpen() const ZINET_API_POST
	{
		return fileStream.is_open();
	}

	bool File::isOkay() const ZINET_API_POST
	{
		return fileStream.good() && !fileStream.fail() && !fileStream.bad();
	}

	void File::log() const ZINET_API_POST
	{
		if (isOkay())
		{
			Logger->info("Everything is okay with the file stream");
		}
		else
		{
			Logger->error("Something is wrong: \nis good: {}, fail: {}, bad: {}",
				fileStream.good(), fileStream.fail(), fileStream.bad());


		}
	}

	std::string File::readLine() ZINET_API_POST
	{
		std::string line;
		std::getline(fileStream, line);
		return line;
	}

	std::string File::readAll() ZINET_API_POST
	{
		fileStream.seekg(0);
		std::string line;
		std::getline(fileStream, line, '\0');
		return line;
	}

	std::vector<File::Byte> File::readData() ZINET_API_POST
	{
		fileStream.seekg(0, std::ios::end);
		std::streamsize size = fileStream.tellg();

		std::vector<Byte> result;
		result.resize(size);

		static_assert(sizeof(Byte) == sizeof(char));

		fileStream.seekg(0, std::ios::beg);
		fileStream.read(reinterpret_cast<char*>(result.data()), size);

		return result;
	}

	void File::write(const std::string& string) ZINET_API_POST
	{
		fileStream << string;
	}

	void File::writeData(const std::vector<Byte>& data) ZINET_API_POST
	{
		fileStream.seekp(0);
		fileStream.write(reinterpret_cast<const char*>(data.data()), data.size());
	}

	std::ios_base::openmode File::ToStdOpenMode(FileOpenMode openMode) ZINET_API_POST
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
				Logger->error("Invalid openMode: {}", static_cast<int>(openMode));
				return std::ios_base::out;
		}
	}

	void File::close() ZINET_API_POST
	{
		fileStream.close();
	}

	bool File::RemoveFile(const std::filesystem::path& path) ZINET_API_POST
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

	File File::CreateFile(const std::filesystem::path& path) ZINET_API_POST
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