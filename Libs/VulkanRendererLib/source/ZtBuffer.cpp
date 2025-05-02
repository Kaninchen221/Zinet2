#include "Zinet/VulkanRenderer/ZtBuffer.hpp"
#include "Zinet/VulkanRenderer/ZtVMA.hpp"

namespace zt::vulkan_renderer
{

	void Buffer::destroy(const VMA& vma) noexcept
	{
		if (isValid())
		{
			vmaDestroyBuffer(vma.get(), objectHandle, allocation);
			objectHandle = nullptr;
			allocation = nullptr;
			size = 0u;
		}
	}

}