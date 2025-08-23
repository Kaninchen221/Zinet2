#pragma once

#include "Zinet/Gameplay/ZtGameplayConfig.hpp"

#include "Zinet/Core/Assets/ZtAssetText.hpp"
#include "Zinet/Core/ZtFile.hpp"
#include "Zinet/Core/ZtPaths.hpp"

#include "Zinet/VulkanRenderer/ZtTexture.hpp"
#include "Zinet/VulkanRenderer/ZtSampler.hpp"
#include "Zinet/VulkanRenderer/ZtDescriptorSets.hpp"

#include "Zinet/Gameplay/Assets/ZtAssetProperty.hpp"
#include "Zinet/Gameplay/Assets/ZtAssetSampler.hpp"

namespace
{
	using Asset = zt::core::Asset;
}

namespace zt::gameplay
{
	class  AssetTexture : public core::Asset
	{
	public:
		AssetTexture(const Extensions& extensions = { "png" }) : core::Asset{ extensions } {}
		AssetTexture(const AssetTexture& other) : core::Asset(other) {}
		AssetTexture(AssetTexture&& other) noexcept = default;
		~AssetTexture() noexcept = default;

		AssetTexture& operator = (const AssetTexture& other) noexcept { Asset::operator =(other); return *this; }
		AssetTexture& operator = (AssetTexture&& other) noexcept = default;

		const std::string_view getClassName() const override { return "zt::gameplay::AssetTexture"; }

		ObjectPtr createCopy() const override { return std::make_unique<AssetTexture>(*this); }

		bool load(const core::Path& rootPath) override;

		void unload() override;

		void imGui() override;

		bool serialize(core::JsonArchive& archive) override;

		bool deserialize(core::JsonArchive& archive) override;

		// TODO: Recreate the descriptor set when the sampler change
		AssetProperty<AssetSampler> sampler{ "Sampler" };

		vulkan_renderer::Texture texture;
		vulkan_renderer::DescriptorSets descriptorSet{ nullptr };
	};

}