#pragma once

#include "Zinet/Gameplay/ZtGameplayConfig.hpp"

#include "Zinet/Core/Assets/ZtAssetText.hpp"

#include "Zinet/VulkanRenderer/ZtShadersCompiler.hpp"

namespace zt::gameplay::asset
{
	class ZINET_GAMEPLAY_API Shader : public core::asset::Text
	{
	public:

		Shader() : Text{ { "frag", "vert" } } { autoLoad = true; }
		Shader(const Shader& other) : Text{other} {}
		Shader(Shader&& other) noexcept = default;
		~Shader() noexcept = default;

		Shader& operator = (const Shader& other) = default;
		Shader& operator = (Shader&& other) noexcept = default;

		const std::string_view getClassName() const override { return "zt::gameplay::assets::Shader"; }

		ObjectPtr createCopy() const override { return std::make_unique<Shader>(*this); }

		bool load(const core::Path& rootPath) override;

		void unload() override;

		void show() override;

		auto& getCompileResult() const { return result; }

	protected:

		vulkan_renderer::ShadersCompiler::CompileResult result;

	};

}