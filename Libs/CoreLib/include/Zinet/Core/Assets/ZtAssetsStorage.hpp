#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"
#include "Zinet/Core/ZtObject.hpp"
#include "Zinet/Core/ZtLogger.hpp"

#include "Zinet/Core/Assets/ZtAssetsFinder.hpp"

#include <unordered_map>
#include <tuple>

namespace zt::core::assets
{
	ZT_REFLECT_CLASS()
	class ZINET_CORE_API AssetsStorage : public Object
	{
	protected:

		inline static auto Logger = ConsoleLogger::Create("AssetsStorage");

	public:

		bool storeAssets() noexcept;

		using AssetsKey = std::string;
		using Assets = std::unordered_map<AssetsKey, Asset>;

		using GetResult = Asset*;
		GetResult get(const AssetsKey& key) noexcept;

		AssetsFinder assetsFinder;

	protected:

		Assets assets;

	public:
/*GENERATED_CODE_START*/
		static_assert(IsObjectClassInherited); // Class using ZT_REFLECT_CLASS should inherit public from Object class
		const inline static bool RegisterClassResult = RegisterClass<AssetsStorage>();
		std::unique_ptr<ObjectBase> createCopy() const override { std::unique_ptr<ObjectBase> result = createCopyInternal<AssetsStorage>(); *result = *this; return result; }
		
		AssetsStorage() = default;
		AssetsStorage(const AssetsStorage& other) = default;
		AssetsStorage(AssetsStorage&& other) = default;
		~AssetsStorage() noexcept = default;
		
		AssetsStorage& operator = (const AssetsStorage& other) = default;
		AssetsStorage& operator = (AssetsStorage&& other) = default;
		
		class ClassInfo : public zt::core::ClassInfoBase
		{
		public:
		
			std::string_view getClassName() const override { return "AssetsStorage"; }
		};
		const zt::core::ClassInfoBase* getClassInfo() const override { static ClassInfo classInfo; return &classInfo; }
		
		
/*GENERATED_CODE_END*/
	};

}