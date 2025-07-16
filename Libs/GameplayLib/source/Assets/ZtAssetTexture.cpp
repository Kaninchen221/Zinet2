#include "Zinet/Gameplay/Assets/ZtAssetTexture.hpp"

namespace zt::gameplay::assets
{
	bool AssetTexture::load([[maybe_unused]] const core::Path& rootPath) ZINET_API_POST
	{
		loaded = true;
		return true;
	}

	void AssetTexture::unload() ZINET_API_POST
	{
		loaded = false;
	}
}

