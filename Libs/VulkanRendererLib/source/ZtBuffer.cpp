#include "Zinet/VulkanRenderer/ZtBuffer.hpp"
#include "Zinet/VulkanRenderer/ZtVMA.hpp"

#include "Zinet/Core/ZtImage.hpp"

namespace zt::vulkan_renderer
{
	VkBufferCreateInfo Buffer::GetImageBufferCreateInfo(const core::Image& image) noexcept
	{
		return
		{
			.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
			.size = static_cast<VkDeviceSize>(image.getSize()),
			.usage = VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			.sharingMode = VK_SHARING_MODE_EXCLUSIVE
		};
	}

	bool Buffer::create(const VMA& vma, const VkBufferCreateInfo& createInfo)
	{
		if (isValid())
			return false;

		const VmaAllocationCreateInfo allocInfo
		{
			.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT,
			.usage = VMA_MEMORY_USAGE_AUTO
		};

		const auto result = vmaCreateBuffer(vma.get(), &createInfo, &allocInfo, &objectHandle, &allocation, nullptr);
		if (result == VK_SUCCESS)
		{
			size = static_cast<std::uint32_t>(createInfo.size);
			return true;
		}
		else
		{
			Logger->error("Couldn't create buffer, result: {}", static_cast<std::int32_t>(result));
			return false;
		}
	}

	bool Buffer::fillWithImage(const VMA& vma, const core::Image& image, size_t bytesOffset)
	{
		if (!isValid())
			return false;

		const auto result = fillWithData(vma, image.getData(), image.getSize(), bytesOffset);

		if (result == VK_SUCCESS)
		{
			return true;
		}
		else
		{
			Logger->error("Couldn't fill buffer, result: {}", static_cast<std::int32_t>(result));
			return false;
		}
	}

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

	VkResult Buffer::fillWithData(const VMA& vma, const void* src, size_t srcSize, size_t bytesDstOffset) const
	{
		if (srcSize + bytesDstOffset > size)
		{
			Logger->error("Invalid size");
			return VK_ERROR_MEMORY_MAP_FAILED;
		}

		std::byte* mappedData{};
		const auto mapResult = vmaMapMemory(vma.get(), allocation, reinterpret_cast<void**>(&mappedData));
		if (mapResult != VK_SUCCESS)
		{
			Logger->error("vmaMapMemory returned non VK_SUCCESS value");
			return mapResult;
		}

		std::memcpy(mappedData + bytesDstOffset, src, srcSize);
		vmaUnmapMemory(vma.get(), allocation);

		return VK_SUCCESS;
	}

	VkResult Buffer::getData(const VMA& vma, void* dst, size_t dstSize, size_t bytesSrcOffset) const noexcept
	{
		if (dstSize + bytesSrcOffset > size)
			return VK_ERROR_MEMORY_MAP_FAILED;

		std::byte* mappedData{};
		const auto mapResult = vmaMapMemory(vma.get(), allocation, reinterpret_cast<void**>(&mappedData));
		if (mapResult != VK_SUCCESS)
			return mapResult;

		std::memcpy(dst, mappedData + bytesSrcOffset, dstSize);
		vmaUnmapMemory(vma.get(), allocation);

		return VK_SUCCESS;
	}

}