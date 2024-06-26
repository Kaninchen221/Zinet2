#pragma once

#include "zinet/lib_name/macros.hpp"
#include "Zinet/Engine/Assets/ZtAsset.h"

#include "Zinet/Core/ZtLogger.h"

ZT_REFLECT_NAMESPACE(Placeholder1)
namespace zt::engine
{
	// We expect that every reflected class has default, copy, move constructors, destructor and copy, move assign operators
	ZT_REFLECT_CLASS(Placeholder1, Placeholder2)
	class ZINET_ENGINE_API TextureAsset : public Asset
	{
		inline static auto Logger = core::ConsoleLogger::Create("TextureAsset");

	public:

        // We need handle structs inside class
        ZT_REFLECT_STRUCT()
        struct CreateInputInfo
        {
		    ZT_REFLECT_MEMBER(ReadOnly)
            int count;
        };

		/* It will be generated in the ZT_GENERATED_BODY()
		TextureAsset() = default;
		TextureAsset(const TextureAsset& other) = default;
		TextureAsset(TextureAsset&& other) = default;

		TextureAsset& operator = (const TextureAsset& other) = default;
		TextureAsset& operator = (TextureAsset&& other) = default;

		~TextureAsset() noexcept = default;
		*/

		// We don't reflect getters and setters. It will be generated via ZT_REFLECT_MEMBER(...)
		// const Texture& getTexture() const { return texture; }
		// void setTexture(const Texture& newTexture) { texture = newTexture; }

		ZT_REFLECT_METHOD()
		void update() { /* ... */ }

		// I think we can get information about return type automatically
		// There will be problem with "is const", "is virtual" information
		ZT_REFLECT_METHOD()
		bool isValid() const { return true; }

	protected:

		ZT_REFLECT_MEMBER(ReadOnly)
		Texture texture;

		ZT_REFLECT_MEMBER(ReadWrite)
		Texture backupTexture;

	public:

/*GENERATED_CODE_START*/
		TextureAsset() = default;
		TextureAsset(const TextureAsset& other) = default;
		TextureAsset(TextureAsset&& other) = default;
		
		~TextureAsset() noexcept = default;
		
		TextureAsset& operator = (const TextureAsset& other) = default;
		TextureAsset& operator = (TextureAsset&& other) = default;
		
		const int& getTexture() const { return texture; }
		
		const int& getBackupTexture() const { return backupTexture; }
		void setBackupTexture(const int& newValue) { backupTexture = newValue; }
		
		class ClassInfo : public zt::core::reflection::ClassInfo
		{
		public:
		
			std::string_view getClassName() const override { return "TextureAsset"; }
			zt::core::reflection::ClassPropertiesInfos getClassPropertiesInfos() override {return zt::core::reflection::ClassPropertiesInfos(std::vector<zt::core::reflection::ClassPropertyInfo>{zt::core::reflection::ClassPropertyInfo{offsetof(CreateInputInfo, count), "count", "int"},
			                                                                                                                                                                                       zt::core::reflection::ClassPropertyInfo{offsetof(TextureAsset, texture), "texture", "int"},
			                                                                                                                                                                                       zt::core::reflection::ClassPropertyInfo{offsetof(TextureAsset, backupTexture), "backupTexture", "int"}}); };
		
		};
		const inline static auto AddClassInfoResult = []()
		{
			auto& classesInfos = zt::core::reflection::ClassesInfos::Get();
			classesInfos.addClassInfo<ClassInfo>();
			return true;
		}();
		std::unique_ptr<zt::core::reflection::ClassInfo> getClassInfoObject() const { return std::make_unique<ClassInfo>(); }
		auto getCopyOfAllMembers() { return std::make_tuple(count, texture, backupTexture); };
		
/*GENERATED_CODE_END*/

	};
}