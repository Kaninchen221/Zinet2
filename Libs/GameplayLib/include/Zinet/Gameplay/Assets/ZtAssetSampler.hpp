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
		using Sampler = vulkan_renderer::Sampler;

		AssetSampler(const Extensions& extensions = { "sampler" }) : Asset{ extensions } {}
		AssetSampler(const AssetSampler& other) : Asset(other) {}
		AssetSampler(AssetSampler&& other) noexcept = default;
		~AssetSampler() noexcept = default;

		AssetSampler& operator = (const AssetSampler& other) { assetSamplerData = other.assetSamplerData; return *this; }
		AssetSampler& operator = (AssetSampler&& other) noexcept = default;

		std::string getClassName() const override { return "zt::gameplay::AssetSampler"; }

		ObjectPtr createCopy() const override 
		{ 
			auto copy = std::make_shared<AssetSampler>(*this);
			copy->autoLoad = true;
			return copy;
		}

		bool load(const core::Path& rootPath) override;

		void unload() override;

		void imGui() override;
		Sampler sampler{ nullptr };

	protected:

		struct {
			std::string typeStr = "Invalid";
		} assetSamplerData;
		std::string& typeStr = assetSamplerData.typeStr;
	};

}