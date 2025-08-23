#include "Zinet/Gameplay/Assets/ZtAssetSampler.hpp"
#include "Zinet/Gameplay/ZtEngineContext.hpp"
#include "Zinet/Gameplay/Systems/ZtSystemRenderer.hpp"

#include "Zinet/Core/ZtFile.hpp"
#include "Zinet/Core/ZtImGui.hpp"

namespace zt::gameplay
{
	bool AssetSampler::load(const core::Path& rootPath)
	{
		if (isLoaded())
			return true;

		core::File file;
		const auto filePath = rootPath / metaData.value("fileRelativePath", "");
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
		const auto samplerType = vulkan_renderer::SamplerTypeFromString(typeStr);
		
		auto createInfo = Sampler::GetDefaultCreateInfo();
		createInfo.magFilter = samplerType;
		createInfo.minFilter = samplerType;

		auto& engineContext = EngineContext::Get();
		auto systemRenderer = engineContext.getSystem<SystemRenderer>();
		if (!systemRenderer)
		{
			Logger->error("Can't create asset sampler because system renderer is invalid");
			return false;
		}
		auto& device = systemRenderer->getRenderer().getRendererContext().device;

		const bool samplerCreated = sampler.create(device, createInfo);
		if (!samplerCreated)
			return false;

		loaded = true;
		return true;
	}

	void AssetSampler::unload()
	{
		if (!isLoaded())
			return;

		auto& engineContext = EngineContext::Get();
		auto systemRenderer = engineContext.getSystem<SystemRenderer>();
		if (!systemRenderer)
		{
			Logger->error("Can't unload asset sampler because system renderer is invalid");
			return;
		}
		auto& device = systemRenderer->getRenderer().getRendererContext().device;
		sampler.destroy(device);

		loaded = false;
	}

	void AssetSampler::show()
	{
		Asset::show();
		if (!isLoaded())
			return;
		
		ImGui::Text("Sampler type:");
		ImGui::SameLine();
		ImGui::Text(typeStr);
	}

}

