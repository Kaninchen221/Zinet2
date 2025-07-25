#pragma once

#include "Zinet/VulkanRenderer/ZtVulkanRendererConfig.hpp"

#include "Zinet/Core/Assets/ZtAsset.hpp"
#include "Zinet/Core/ZtFile.hpp"

namespace zt::vulkan_renderer::assets
{
	class ZINET_VULKAN_RENDERER_API AssetShader : public core::assets::Asset
	{
	public:

		AssetShader() : Asset{ { "frag", "vert" }} {}
		AssetShader(const AssetShader& other) ZINET_API_POST = default;
		AssetShader(AssetShader&& other) ZINET_API_POST = default;
		~AssetShader() ZINET_API_POST = default;

		AssetShader& operator = (const AssetShader& other) ZINET_API_POST = default;
		AssetShader& operator = (AssetShader&& other) ZINET_API_POST = default;

		AssetPtr createCopy() const ZINET_API_POST override { return std::make_unique<AssetShader>(*this); }

		bool load(const core::Path& rootPath) ZINET_API_POST override;

		void unload() ZINET_API_POST override;

		void imGui() ZINET_API_POST override;

		// Content
		std::string shaderCode;

	};

}