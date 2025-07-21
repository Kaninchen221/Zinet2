#include "Zinet/Core/ZtImage.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <utility>

namespace zt::core
{
	Image& Image::operator=(Image&& other)
	{
		imageData = std::move(other.imageData);
		width = other.width;
		other.width = {};
		height = other.height;
		other.height = {};
		components = other.components;
		other.components = {};

		return *this;
	}

	bool Image::loadFromFile(const fs::path& path, int32_t expectedComponents) ZINET_API_POST
	{
		if (!fs::exists(path))
		{
			Logger->error("Can't load image from file, path doesn't exists: {}", path.string());
			return false;
		}

		const auto pathAsString = path.string();
		stbi_uc* stbiRawDataPtr = stbi_load(pathAsString.c_str(), &width, &height, &components, expectedComponents);

		imageData = ImageDataT{ stbiRawDataPtr, stbiFree };

		if (!imageData)
		{
			Logger->error("Couldn't load image from file, stbi returned nullptr, error message: {}", stbi_failure_reason());
			return false;
		}

		return true;
	}

	bool Image::loadFromData(const Data& data, int32_t expectedComponents) ZINET_API_POST
	{
		if (data.empty())
		{
			Logger->error("Data is empty");
			return false;
		}

		const auto size = static_cast<int>(data.size());
		stbi_uc* stbiRawDataPtr = stbi_load_from_memory(reinterpret_cast<const stbi_uc*>(data.data()), size, &width, &height, &components, expectedComponents);

		imageData = ImageDataT{ stbiRawDataPtr, stbiFree };

		if (!imageData)
		{
			Logger->error("Couldn't load image from data, stbi returned nullptr, error message: {}", stbi_failure_reason());
			return false;
		}

		return true;
	}

	void Image::destroy() ZINET_API_POST
	{
		imageData.reset();
		width = {};
		height = {};
		components = {};
	}

}