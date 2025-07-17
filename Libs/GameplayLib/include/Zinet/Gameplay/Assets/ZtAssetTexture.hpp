#pragma once

#include "Zinet/Core/Assets/ZtAsset.hpp"
#include "Zinet/Core/ZtFile.hpp"
#include "Zinet/Core/ZtPaths.hpp"

#include "Zinet/VulkanRenderer/ZtTexture.hpp"
#include "Zinet/VulkanRenderer/ZtSampler.hpp"
#include "Zinet/VulkanRenderer/ZtDescriptorSets.hpp"

namespace
{
	using Asset = zt::core::assets::Asset;
	template<class AssetT = Asset>
	using AssetHandle = zt::core::assets::AssetHandle<AssetT>;
}

namespace zt::gameplay::assets
{
	class ZINET_CORE_API AssetTexture : public Asset
	{
	public:
		AssetTexture() : Asset{ { "png" } } {}
		AssetTexture(const AssetTexture& other) ZINET_API_POST { Asset::Asset(other); }
		AssetTexture(AssetTexture&& other) ZINET_API_POST = default;
		~AssetTexture() ZINET_API_POST { unload(); }

		AssetTexture& operator = (const AssetTexture& other) ZINET_API_POST { Asset::operator =(other); return *this; }
		AssetTexture& operator = (AssetTexture&& other) ZINET_API_POST = default;

		AssetHandle<Asset> createCopy() const ZINET_API_POST override { return std::make_shared<AssetTexture>(*this); }

		bool load(const core::Path& rootPath) ZINET_API_POST override;

		void unload() ZINET_API_POST override;

		void imGuiAssetInspect() ZINET_API_POST override;

		vulkan_renderer::Texture texture;
		// TODO: AssetTexture should contains an AssetSampler?
		vulkan_renderer::Sampler sampler{ nullptr };
		vulkan_renderer::DescriptorSets descriptorSet{ nullptr };
	};

}