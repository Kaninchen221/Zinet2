#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"
#include "Zinet/Core/ZtObject.hpp"
#include "Zinet/Core/ZtLogger.hpp"
#include "Zinet/Core/ZtPaths.hpp"
#include "Zinet/Core/ZtArchive.hpp"

#include <filesystem>

#include <nlohmann/json.hpp>

namespace zt::core
{
	class Asset : public Object
	{
	protected:

		inline static auto Logger = ConsoleLogger::Create("Asset");

	public:

		using Extensions = std::vector<std::string>;

		Asset() = default;
		Asset(const Extensions& newExtensions) : extensions{ newExtensions } {}
		Asset(const Asset& other) = default;
		Asset(Asset&& other) noexcept = default;
		~Asset() noexcept = default;

		Asset& operator = (const Asset& other) = default;
		Asset& operator = (Asset&& other) noexcept = default;

		std::string getDisplayName() { return metaData.value("fileNameExt", "fileNameExt_DefaultName"); }

		bool isLoaded() noexcept { return loaded; }

		virtual bool load([[maybe_unused]] const Path& rootPath) { return false; }

		virtual void unload() {}

		const auto& getExtensions() const noexcept { return extensions; }
		
		virtual void show();

		bool getAutoLoad() const noexcept { return autoLoad; }

		auto& getMetaData() noexcept { return metaData; }
		void setMetaData(auto&& newMetaData) { metaData = newMetaData; }

	protected:

		bool loaded = false;
		bool autoLoad = false;

		Json metaData;

	private:

		Extensions extensions = { "default_ext" };

	};

}