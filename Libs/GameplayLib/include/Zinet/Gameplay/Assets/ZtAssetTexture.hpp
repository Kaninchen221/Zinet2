#pragma once

#include "Zinet/Gameplay/ZtGameplayConfig.hpp"

#include "Zinet/Core/Assets/ZtAssetText.hpp"

#include "Zinet/Core/ZtFile.hpp"
#include "Zinet/Core/ZtPaths.hpp"
#include "Zinet/Core/ZtImage.hpp"

#include "Zinet/VulkanRenderer/ZtRendererContext.hpp"
#include "Zinet/VulkanRenderer/ZtTexture.hpp"

namespace zt::gameplay::asset
{
	class ZINET_GAMEPLAY_API Texture : public core::Asset
	{
	public:

		using ResourceOptT = std::optional<vulkan_renderer::Texture>;

		Texture(const Extensions& extensions = { "png" }) : core::Asset{ extensions } {}
		Texture(const Texture& other) : core::Asset(other) {}
		Texture(Texture&& other) noexcept = default;
		~Texture() noexcept = default;

		Texture& operator = (const Texture& other) noexcept { Asset::operator =(other); return *this; }
		Texture& operator = (Texture&& other) noexcept = default;

		const std::string_view getClassName() const override { return "zt::gameplay::assets::Texture"; }

		ObjectPtr createCopy() const override { return std::make_unique<Texture>(*this); }

		bool load(const core::Path& rootPath) override;

		void unload() override;

		auto& getImage() const noexcept { return image; }

		// TODO: Test it
		ResourceOptT createResource(vulkan_renderer::RendererContext& rendererContext);

	protected:

		core::Image image;

	};

}