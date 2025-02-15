#include "Zinet/Core/Assets/ZtAsset.hpp"
#include "Zinet/Core/ZtPointersUtilities.hpp"

namespace zt::core::assets
{

	std::unique_ptr<Asset> Asset::createObject(const std::filesystem::path& contentRootFolder) const
	{
		if (metaData.is_null())
		{
			Logger->error("Can't create object, meta data is null");
			return {};
		}

		auto findIt = metaData.find("fileExtension");
		if (findIt == metaData.end())
		{
			Logger->error("Can't create object, can't find file extension in meta data");
			return {};
		}

		const std::string file_extension = *findIt;

		const ClassDefaultObjectRegistry& cdoRegistry = ClassDefaultObjectRegistry::Get();
		const auto& cdos = cdoRegistry.getCDOs();
		for (const auto& cdo : cdos)
		{
			if (!cdo->canBeCreatedFromAsset())
				continue;

			if (cdo->getAssetExtension() != file_extension)
				continue;

			auto createdCopy = cdo->createCopy();
			if (!createdCopy)
			{
				Logger->critical("CDO createCopy returned invalid ptr");
				return {};
			}
			
			auto asset = PointersUtilities::StaticCastUniquePtr<ObjectBase, Asset>(createdCopy);
			asset->metaData = metaData;
			return asset;
		}

		Logger->error("Couldn't find a CDO that has wanted extension: {}", file_extension);
		return {};
	}

}