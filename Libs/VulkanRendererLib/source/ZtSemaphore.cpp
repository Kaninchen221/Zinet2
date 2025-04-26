#include "Zinet/VulkanRenderer/ZtSemaphore.hpp"
#include "Zinet/VulkanRenderer/ZtDevice.hpp"

namespace zt::vulkan_renderer
{

	bool Semaphore::create(const Device& device) noexcept
	{
		if (isValid())
			return false;

		VkSemaphoreCreateInfo semaphoreInfo{};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		const auto result = vkCreateSemaphore(device.get(), &semaphoreInfo, nullptr, &objectHandle);
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
			vkDestroySemaphore(device.get(), objectHandle, nullptr);
			objectHandle = nullptr;
		}
	}

}