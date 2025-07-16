#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"
#include "Zinet/Core/ZtObject.hpp"
#include "Zinet/Core/ZtLogger.hpp"
#include "Zinet/Core/ZtPaths.hpp"

#include <filesystem>

#include <nlohmann/json.hpp>

namespace zt::core::assets
{
	class Asset;

	template<std::derived_from<Asset> AssetT = Asset>
	using AssetHandle = std::shared_ptr<AssetT>;

	ZT_REFLECT_CLASS()
	class ZINET_CORE_API Asset : public Object
	{
	protected:

		inline static auto Logger = ConsoleLogger::Create("Asset");

	public:

		using json = nlohmann::json;

		Asset() ZINET_API_POST = default;
		Asset(std::string_view newExtension) : extension{newExtension} {}
		Asset(const Asset& other) ZINET_API_POST = default;
		Asset(Asset&& other) ZINET_API_POST = default;
		~Asset() ZINET_API_POST = default;

		Asset& operator = (const Asset& other) ZINET_API_POST = default;
		Asset& operator = (Asset&& other) ZINET_API_POST = default;

		virtual AssetHandle<Asset> createCopy() const ZINET_API_POST { return {}; }

		virtual bool load([[maybe_unused]] const Path& rootPath) ZINET_API_POST { return false; }

		virtual void unload() ZINET_API_POST {}

		const auto& getExtension() const ZINET_API_POST { return extension; }

		json metaData;

	private:

		// Config
		std::string extension = "default_ext";

	};

}