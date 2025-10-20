#include "Zinet/Gameplay/Assets/ZtAssetTexture.hpp"

#include "Zinet/Core/ZtFile.hpp"
#include "Zinet/Core/ZtImgui.hpp"

namespace zt::gameplay::asset
{
	bool Texture::load(const core::Path& rootPath)
	{
		if (isLoaded())
			return true;

		core::File file;
		const auto filePath = rootPath / metaData.value("fileRelativePath", "");
		file.open(filePath, core::FileOpenMode::Read, true);
		if (!file.isOpen())
		{
			Logger->error("Couldn't open file, path: {}", filePath.generic_string());
			return false;
		}

		if (!file.isOkay())
		{
			Logger->error("Something went wrong during reading the file");
			return false;
		}

		const auto rawData = file.readData();
		file.close();

		if (!image.loadFromData(rawData, 4))
		{
			Logger->error("Couldn't load an image from a data");
			return false;
		}

		loaded = true;
		return true;
	}

	void Texture::unload()
	{
		if (!isLoaded())
			return;

		image.destroy();

		loaded = false;
	}

}

