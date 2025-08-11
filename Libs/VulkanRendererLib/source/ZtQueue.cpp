#include "Zinet/VulkanRenderer/ZtQueue.hpp"

namespace zt::vulkan_renderer
{

	bool Queue::waitIdle() const
	{
		const auto result = vkQueueWaitIdle(get());
		if (result == VK_SUCCESS)
		{
			return true;
		}
		else
		{
			Logger->error("Queue wait idle returned not success value: {}",
				static_cast<std::int32_t>(result));
			return false;
		}
	}

}