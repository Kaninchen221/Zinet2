#include "Zinet/Gameplay/Assets/ZtAssetSampler.hpp"

#include "Zinet/VulkanRenderer/ZtRendererContext.hpp"

#include "Zinet/Core/ZtFile.hpp"

namespace zt::gameplay::asset
{
	bool Sampler::load()
	{
		if (isLoaded())
			return true;

		core::File file;
		const auto filePath = core::Paths::RootPath() / metaData.value("fileRelativePath", "");
		file.open(filePath, core::FileOpenMode::Read, true);
		if (!file.isOpen())
		{
			Logger->error("Couldn't open file, path: {}", filePath.generic_string());
			return false;
		}

		if (!file.isOkay())
		{
			Logger->error("Something went wrong during reading the file");
			return false;
		}

		const auto text = file.readAll();
		file.close();

		using Json = nlohmann::json;
		Json json = Json::parse(text);
		typeStr = json.value("type", "linear");

		loaded = true;
		return true;
	}

	void Sampler::unload()
	{
		typeStr.clear();
		loaded = false;
	}

	Sampler::ResourceOptT Sampler::createResource(vulkan_renderer::RendererContext& rendererContext)
	{
		vulkan_renderer::Sampler sampler{ nullptr };
		auto createInfo = vulkan_renderer::Sampler::GetDefaultCreateInfo();

		auto filter = vulkan_renderer::SamplerTypeFromString(typeStr);
		createInfo.magFilter = filter;
		createInfo.minFilter = filter;

		if (!sampler.create(rendererContext.getDevice(), createInfo))
			return { nullptr };

		return sampler;
	}
}

