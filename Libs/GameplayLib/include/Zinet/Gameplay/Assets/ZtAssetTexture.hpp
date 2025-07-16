#pragma once

#include "Zinet/Core/Assets/ZtAsset.hpp"
#include "Zinet/Core/ZtFile.hpp"
#include "Zinet/Core/ZtPaths.hpp"

namespace
{
	using Asset = zt::core::assets::Asset;
	template<class AssetT = Asset>
	using AssetHandle = zt::core::assets::AssetHandle<AssetT>;
}

namespace zt::gameplay::assets
{
	class ZINET_CORE_API AssetTexture : public Asset
	{
	public:
		AssetTexture() : Asset{ { "png" } } {}
		AssetTexture(const AssetTexture& other) ZINET_API_POST = default;
		AssetTexture(AssetTexture&& other) ZINET_API_POST = default;
		~AssetTexture() ZINET_API_POST = default;

		AssetTexture& operator = (const AssetTexture& other) ZINET_API_POST = default;
		AssetTexture& operator = (AssetTexture&& other) ZINET_API_POST = default;

		AssetHandle<Asset> createCopy() const ZINET_API_POST override { return std::make_shared<AssetTexture>(*this); }

		bool load(const core::Path& rootPath) ZINET_API_POST override;

		void unload() ZINET_API_POST override;
	};

}