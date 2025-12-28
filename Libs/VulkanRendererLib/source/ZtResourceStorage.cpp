#include "Zinet/VulkanRenderer/ZtResourceStorage.hpp"

namespace zt::vulkan_renderer
{
	bool ResourceStorage::createResources(RendererContext& rendererContext)
	{
		bool anyRequestFailed = false;
		for (auto request : requests)
		{
			if (!request)
			{
				Logger->critical("Invalid request");
				continue;
			}

			if (!std::invoke(request, rendererContext))
				anyRequestFailed = true;
		}

		requests.clear();
		return !anyRequestFailed;
	}

	void ResourceStorage::clear(const RendererContext& rendererContext)
	{
		for (auto& resourceDecorator : resources)
		{
			auto& handles = resourceDecorator.handles;
			auto& destroyFunc = resourceDecorator.destroyFunc;

			if (!destroyFunc)
			{
				Logger->critical("ResourceDecorator has no destroy function");
				continue;
			}

			for (auto handlePtr : handles)
			{
				destroyFunc(handlePtr, rendererContext);
			}
		}
	}

}
