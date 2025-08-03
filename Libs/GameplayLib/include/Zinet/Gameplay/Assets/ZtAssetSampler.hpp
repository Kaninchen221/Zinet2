#pragma once

#include "Zinet/Gameplay/ZtGameplayConfig.hpp"

#include "Zinet/Core/Assets/ZtAsset.hpp"
#include "Zinet/Core/ZtPaths.hpp"

#include "Zinet/VulkanRenderer/ZtSampler.hpp"

namespace
{
	using Asset = zt::core::Asset;
	template<class AssetT = Asset>
	using AssetHandle = zt::core::AssetHandle<AssetT>;
}

namespace zt::gameplay
{
	class ZINET_GAMEPLAY_API AssetSampler : public Asset
	{
	public:
		AssetSampler(const Extensions& extensions = { "sampler" }) : Asset{ extensions } {}
		AssetSampler(const AssetSampler& other) ZINET_API_POST : Asset(other) {}
		AssetSampler(AssetSampler&& other) ZINET_API_POST = default;
		~AssetSampler() ZINET_API_POST = default;

		AssetSampler& operator = (const AssetSampler& other) ZINET_API_POST { Asset::operator =(other); return *this; }
		AssetSampler& operator = (AssetSampler&& other) ZINET_API_POST = default;

		AssetPtr createCopy() const ZINET_API_POST override 
		{ 
			auto copy = std::make_shared<AssetSampler>(*this);
			copy->autoLoad = true;
			return copy;
		}

		bool load(const core::Path& rootPath) ZINET_API_POST override;

		void unload() ZINET_API_POST override;

		void imGui() ZINET_API_POST override;

		using Sampler = vulkan_renderer::Sampler;
		Sampler sampler{ nullptr };

	protected:

		std::string typeStr = "Invalid";

	};

}