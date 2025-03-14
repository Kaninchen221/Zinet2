#pragma once

#include "Zinet/Core/Assets/ZtAsset.hpp"

namespace zt::core::assets
{
	ZT_REFLECT_CLASS()
	class ZINET_CORE_API TextAsset : public Asset
	{
	public:

		bool canBeCreatedFromAsset() const override { return true; }

		std::string_view getAssetExtension() const override { return "txt"; }

		bool loadContentUsingMetaData(const std::filesystem::path& contentRootFolder) override;

	protected:

		ZT_REFLECT_MEMBER(ReadWrite)
		std::string text;

	public:
/*GENERATED_CODE_START*/
		static_assert(IsObjectClassInherited); // Class using ZT_REFLECT_CLASS should inherit public from Object class
		const inline static bool RegisterClassResult = RegisterClass<TextAsset>();
		std::unique_ptr<ObjectBase> createCopy() const override { std::unique_ptr<ObjectBase> result = createCopyInternal<TextAsset>(); *result = *this; return result; }
		
		TextAsset() = default;
		TextAsset(const TextAsset& other) = default;
		TextAsset(TextAsset&& other) = default;
		~TextAsset() noexcept = default;
		
		TextAsset& operator = (const TextAsset& other) = default;
		TextAsset& operator = (TextAsset&& other) = default;
		
		class ClassInfo : public zt::core::ClassInfoBase
		{
		public:
		
			std::string_view getClassName() const override { return "TextAsset"; }
		};
		const zt::core::ClassInfoBase* getClassInfo() const override { static ClassInfo classInfo; return &classInfo; }
		
		
		const decltype(text)& getText() const { return text; }
		void setText(const decltype(text)& newValue) { text = newValue; }
		
/*GENERATED_CODE_END*/
	};
}