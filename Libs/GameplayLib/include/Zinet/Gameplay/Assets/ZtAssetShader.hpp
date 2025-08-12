#pragma once

#include "Zinet/Gameplay/ZtGameplayConfig.hpp"

#include "Zinet/VulkanRenderer/ZtShaderModule.hpp"

#include "Zinet/Core/Assets/ZtAssetText.hpp"
#include "Zinet/Core/ZtFile.hpp"

namespace zt::gameplay
{
	class ZINET_GAMEPLAY_API AssetShader : public core::AssetText
	{
	public:

		AssetShader() : AssetText{ { "frag", "vert" }} {}
		AssetShader(const AssetShader& other) : AssetText{other} {}
		AssetShader(AssetShader&& other) noexcept = default;
		~AssetShader() noexcept = default;

		AssetShader& operator = (const AssetShader& other) = default;
		AssetShader& operator = (AssetShader&& other) noexcept = default;

		std::string getClassName() const override { return "zt::gameplay::AssetShader"; }

		ObjectPtr createCopy() const override { return std::make_unique<AssetShader>(*this); }

		bool load(const core::Path& rootPath) override;

		void unload() override;

		void imGui() override;

		vulkan_renderer::ShaderModule shaderModule{ nullptr };
	};

}