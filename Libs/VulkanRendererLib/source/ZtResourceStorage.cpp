#include "Zinet/VulkanRenderer/ZtResourceStorage.hpp"

namespace zt::vulkan_renderer
{
	bool ResourceStorage::createResources(const RendererContext& rendererContext)
	{
		bool anyRequestFailed = false;
		for (auto request : requests)
		{
			if (!std::invoke(request, rendererContext))
				anyRequestFailed = true;
		}

		requests.clear();
		return !anyRequestFailed;
	}
}
