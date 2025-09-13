#include "Zinet/VulkanRenderer/ZtSemaphore.hpp"
#include "Zinet/VulkanRenderer/ZtDevice.hpp"

namespace zt::vulkan_renderer
{

	bool Semaphore::create(const Device& device)
	{
		if (isValid())
			return false;

		const VkSemaphoreCreateInfo createInfo
		{
			.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
			.pNext = nullptr,
			.flags = {}
		};

		const auto result = vkCreateSemaphore(device.get(), &createInfo, nullptr, &get());
		if (result == VK_SUCCESS)
		{
			return true;
		}
		else
		{
			Logger->error("Couldn't create semaphore, result: {}", static_cast<std::int32_t>(result));
			return false;
		}
	}

	void Semaphore::destroy(const Device& device) noexcept
	{
		if (isValid())
		{
			vkDestroySemaphore(device.get(), get(), nullptr);
			invalidateInternal();
		}
	}

}