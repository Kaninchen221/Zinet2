#include "Zinet/Core/ZtImage.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <utility>

namespace zt::core
{

	bool Image::loadFromFile(const fs::path& path, std::int32_t expectedComponents) noexcept
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

	void Image::destroy() noexcept
	{
		imageData.reset();
		width = {};
		height = {};
		components = {};
	}

}