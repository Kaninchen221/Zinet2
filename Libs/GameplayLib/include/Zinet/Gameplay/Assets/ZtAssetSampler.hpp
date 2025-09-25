#pragma once

#include "Zinet/Gameplay/ZtGameplayConfig.hpp"

#include "Zinet/Core/Assets/ZtAsset.hpp"
#include "Zinet/Core/ZtPaths.hpp"

#include "Zinet/VulkanRenderer/ZtSampler.hpp"

namespace
{
	using Asset = zt::core::Asset;
}

namespace zt::gameplay
{
	class  AssetSampler : public Asset
	{
	public:
		using Sampler = vulkan_renderer::Sampler;

		AssetSampler(const Extensions& extensions = { "sampler" }) : Asset{ extensions } {}
		AssetSampler(const AssetSampler& other) : Asset(other) {}
		AssetSampler(AssetSampler&& other) noexcept = default;
		~AssetSampler() noexcept = default;

		AssetSampler& operator = (const AssetSampler& other) { typeStr = other.typeStr; return *this; }
		AssetSampler& operator = (AssetSampler&& other) noexcept = default;

		const std::string_view getClassName() const override { return "zt::gameplay::AssetSampler"; }

		ObjectPtr createCopy() const override 
		{ 
			auto copy = std::make_unique<AssetSampler>(*this);
			copy->autoLoad = true;
			return copy;
		}

		bool load(const core::Path& rootPath) override;

		void unload() override;

		void show() override;

		// TODO: Add getter
		Sampler sampler{ nullptr };

	protected:

		std::string typeStr = "Invalid";
	};

}