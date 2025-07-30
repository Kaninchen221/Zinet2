#pragma once

#include "Zinet/Gameplay/ZtGameplayConfig.hpp"

#include "Zinet/VulkanRenderer/ZtShaderModule.hpp"

#include "Zinet/Core/Assets/ZtAssetText.hpp"
#include "Zinet/Core/ZtFile.hpp"

namespace zt::gameplay::assets
{
	class ZINET_GAMEPLAY_API AssetShader : public core::assets::AssetText
	{
	public:

		AssetShader() : AssetText{ { "frag", "vert" }} {}
		AssetShader(const AssetShader& other) ZINET_API_POST : AssetText{other} {}
		AssetShader(AssetShader&& other) ZINET_API_POST = default;
		~AssetShader() ZINET_API_POST = default;

		AssetShader& operator = (const AssetShader& other) ZINET_API_POST = default;
		AssetShader& operator = (AssetShader&& other) ZINET_API_POST = default;

		AssetPtr createCopy() const ZINET_API_POST override { return std::make_unique<AssetShader>(*this); }

		bool load(const core::Path& rootPath) ZINET_API_POST override;

		void unload() ZINET_API_POST override;

		void imGui() ZINET_API_POST override;

		vulkan_renderer::ShaderModule shaderModule{ nullptr };
	};

}