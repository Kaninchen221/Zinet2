#pragma once

#include "Zinet/Core/Assets/ZtAsset.hpp"
#include "Zinet/Core/ZtFile.hpp"

namespace zt::core
{
	class AssetText : public Asset
	{
	public:

		AssetText(const Extensions& extensions = { "txt" }) : Asset{ extensions } {}
		AssetText(const AssetText& other) = default;
		AssetText(AssetText&& other) noexcept = default;
		~AssetText() noexcept = default;

		AssetText& operator = (const AssetText& other) = default;
		AssetText& operator = (AssetText&& other) noexcept = default;

		const std::string_view getClassName() const override { return "zt::core::AssetText"; }

		ObjectPtr createCopy() const override { return std::make_unique<AssetText>(*this); }

		bool load(const Path& rootPath) override;

		void unload() override;

		void show() override;

		const std::string& getText() const { return text; }

	protected:

		// Content
		std::string text;

	};

}