#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"
#include "Zinet/Core/ZtFile.hpp"

#include "Zinet/Core/Assets/ZtAsset.hpp"

namespace zt::core::asset
{
	class ZINET_CORE_API Text : public Asset
	{
	public:

		Text(const Extensions& extensions = { "txt" }) : Asset{ extensions } {}
		Text(const Text& other) = default;
		Text(Text&& other) noexcept = default;
		~Text() noexcept = default;

		Text& operator = (const Text& other) = default;
		Text& operator = (Text&& other) noexcept = default;

		const std::string_view getClassName() const override { return "zt::core::assets::Text"; }

		ObjectPtr createCopy() const override { return std::make_unique<Text>(*this); }

		bool load(const Path& rootPath) override;

		void unload() override;

		void show() override;

		const std::string& getText() const { return text; }

	protected:

		std::string text;

	};

}