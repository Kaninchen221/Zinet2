#pragma once

#include "Zinet/Core/Assets/ZtAsset.hpp"
#include "Zinet/Core/ZtFile.hpp"

namespace zt::core::assets
{
	class ZINET_CORE_API AssetText : public Asset
	{
	public:

		AssetText() : Asset{ { "txt" } } {}
		AssetText(const AssetText& other) ZINET_API_POST = default;
		AssetText(AssetText&& other) ZINET_API_POST = default;
		~AssetText() ZINET_API_POST = default;

		AssetText& operator = (const AssetText& other) ZINET_API_POST = default;
		AssetText& operator = (AssetText&& other) ZINET_API_POST = default;

		AssetHandle<Asset> createCopy() const ZINET_API_POST override { return std::make_shared<AssetText>(*this); }

		bool load(const Path& rootPath) ZINET_API_POST override;

		void unload() ZINET_API_POST override;

		// Content
		std::string text;

	};

}