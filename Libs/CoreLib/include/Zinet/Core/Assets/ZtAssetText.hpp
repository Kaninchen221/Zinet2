#pragma once

#include "Zinet/Core/Assets/ZtAsset.hpp"
#include "Zinet/Core/ZtFile.hpp"

namespace zt::core
{
	class ZINET_CORE_API AssetText : public Asset
	{
	public:

		AssetText(const Extensions& extensions = { "txt" }) : Asset{ extensions } {}
		AssetText(const AssetText& other) ZINET_API_POST = default;
		AssetText(AssetText&& other) ZINET_API_POST = default;
		~AssetText() ZINET_API_POST = default;

		AssetText& operator = (const AssetText& other) ZINET_API_POST = default;
		AssetText& operator = (AssetText&& other) ZINET_API_POST = default;

		std::string getClassName() const ZINET_API_POST override { return "zt::core::AssetText"; }

		AssetPtr createCopy() const ZINET_API_POST override { return std::make_unique<AssetText>(*this); }

		bool load(const Path& rootPath) ZINET_API_POST override;

		void unload() ZINET_API_POST override;

		void imGui() ZINET_API_POST override;

		// Content
		std::string text;

	};

}