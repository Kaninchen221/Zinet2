#pragma once

#include "Zinet/Gameplay/ZtGameplayConfig.hpp"

#include "Zinet/Core/Assets/ZtAsset.hpp"
#include "Zinet/Core/ZtPaths.hpp"

#include "Zinet/VulkanRenderer/ZtSampler.hpp"

namespace zt::vulkan_renderer
{
	class RendererContext;
}

namespace zt::gameplay::asset
{
	class ZINET_GAMEPLAY_API Sampler : public core::Asset
	{
	public:
		using ResourceOptT = std::optional<vulkan_renderer::Sampler>;

		Sampler(const Extensions& extensions = { "sampler" }) : Asset{ extensions } {}
		Sampler(const Sampler& other) : Asset(other) {}
		Sampler(Sampler&& other) noexcept = default;
		~Sampler() noexcept = default;

		Sampler& operator = (const Sampler& other) { typeStr = other.typeStr; return *this; }
		Sampler& operator = (Sampler&& other) noexcept = default;

		const std::string_view getClassName() const override { return "zt::gameplay::Sampler"; }

		bool load(const core::Path& rootPath);

		void unload();

		ObjectPtr createCopy() const override 
		{ 
			auto copy = std::make_unique<Sampler>(*this);
			copy->autoLoad = true;
			return copy;
		}

		auto& getTypeString() const { return typeStr; }

		// TODO: Test it
		ResourceOptT createResource(vulkan_renderer::RendererContext& rendererContext);

	protected:

		std::string typeStr = "Invalid";
	};

}