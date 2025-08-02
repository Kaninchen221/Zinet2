#pragma once

#include "Zinet/Gameplay/ZtGameplayConfig.hpp"

#include "Zinet/Core/Assets/ZtAssetText.hpp"
#include "Zinet/Core/Assets/ZtAssetProperty.hpp"
#include "Zinet/Core/ZtFile.hpp"
#include "Zinet/Core/ZtPaths.hpp"

#include "Zinet/VulkanRenderer/ZtTexture.hpp"
#include "Zinet/VulkanRenderer/ZtSampler.hpp"
#include "Zinet/VulkanRenderer/ZtDescriptorSets.hpp"

#include "Zinet/Gameplay/Assets/ZtAssetSampler.hpp"

namespace
{
	using Asset = zt::core::Asset;
	template<class AssetT = Asset>
	using AssetHandle = zt::core::AssetHandle<AssetT>;
}

namespace zt::gameplay
{
	class ZINET_GAMEPLAY_API AssetTexture : public core::Asset
	{
	public:
		AssetTexture(const Extensions& extensions = { "png" }) : core::Asset{ extensions } {}
		AssetTexture(const AssetTexture& other) ZINET_API_POST { Asset::Asset(other); }
		AssetTexture(AssetTexture&& other) ZINET_API_POST = default;
		~AssetTexture() ZINET_API_POST = default;

		AssetTexture& operator = (const AssetTexture& other) ZINET_API_POST { Asset::operator =(other); return *this; }
		AssetTexture& operator = (AssetTexture&& other) ZINET_API_POST = default;

		AssetPtr createCopy() const ZINET_API_POST override { return std::make_shared<AssetTexture>(*this); }

		bool load(const core::Path& rootPath) ZINET_API_POST override;

		void unload() ZINET_API_POST override;

		void imGui() ZINET_API_POST override;

		// TODO: Recreate the descriptor set when the sampler change
		core::AssetProperty<AssetSampler> sampler{ "Sampler" };

		vulkan_renderer::Texture texture;
		vulkan_renderer::DescriptorSets descriptorSet{ nullptr };
	};

}