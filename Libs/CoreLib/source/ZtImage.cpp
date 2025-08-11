#include "Zinet/Core/ZtImage.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <utility>

namespace zt::core
{
	Image& Image::operator=(Image&& other) noexcept
	{
		data = std::move(other.data);
		other.data = {};
		width = other.width;
		other.width = {};
		height = other.height;
		other.height = {};
		components = other.components;
		other.components = {};

		return *this;
	}

	bool Image::loadFromFile(const Path& path, int32_t expectedComponents)
	{
		using namespace std::filesystem;

		if (!exists(path))
		{
			Logger->error("Can't load image from file, path doesn't exists: {}", path.string());
			return false;
		}

		const auto pathAsString = path.string();
		data = stbi_load(pathAsString.c_str(), &width, &height, &components, expectedComponents);

		if (!data)
		{
			Logger->error("Couldn't load image from file, stbi returned nullptr, error message: {}", stbi_failure_reason());
			return false;
		}

		return true;
	}

	bool Image::loadFromData(const Data& rawData, int32_t expectedComponents)
	{
		if (rawData.empty())
		{
			Logger->error("Data is empty");
			return false;
		}

		const auto size = static_cast<int>(rawData.size());
		data = stbi_load_from_memory(reinterpret_cast<const stbi_uc*>(rawData.data()), size, &width, &height, &components, expectedComponents);

		if (!data)
		{
			Logger->error("Couldn't load image from data, stbi returned nullptr, error message: {}", stbi_failure_reason());
			return false;
		}

		return true;
	}

	void Image::destroy() noexcept
	{
		if (!data)
			return;

		stbi_image_free(data);
		data = {};
		width = {};
		height = {};
		components = {};
	}

}