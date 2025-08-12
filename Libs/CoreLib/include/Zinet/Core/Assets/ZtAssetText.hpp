#pragma once

#include "Zinet/Core/Assets/ZtAsset.hpp"
#include "Zinet/Core/ZtFile.hpp"

namespace zt::core
{
	class AssetText : public Asset
	{
	public:

		ZINET_CORE_API AssetText(const Extensions& extensions = { "txt" }) : Asset{ extensions } {}
		ZINET_CORE_API AssetText(const AssetText& other) = default;
		ZINET_CORE_API AssetText(AssetText&& other) noexcept = default;
		ZINET_CORE_API ~AssetText() noexcept = default;

		ZINET_CORE_API AssetText& operator = (const AssetText& other) = default;
		ZINET_CORE_API AssetText& operator = (AssetText&& other) noexcept = default;

		ZINET_CORE_API std::string getClassName() const override { return "zt::core::AssetText"; }

		ZINET_CORE_API ObjectPtr createCopy() const override { return std::make_unique<AssetText>(*this); }

		ZINET_CORE_API bool load(const Path& rootPath) override;

		ZINET_CORE_API void unload() override;

		ZINET_CORE_API void imGui() override;

		ZINET_CORE_API std::string getText() const { return text; }

	protected:

		// Content
		std::string text;

	};

}