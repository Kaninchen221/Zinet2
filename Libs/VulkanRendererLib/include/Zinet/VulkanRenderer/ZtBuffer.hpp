#pragma once

#include "Zinet/VulkanRenderer/ZtVulkanRendererConfig.hpp"
#include "Zinet/VulkanRenderer/ZtVulkanObject.hpp"
#include "Zinet/VulkanRenderer/ZtVertex.hpp"
#include "Zinet/VulkanRenderer/ZtVMA.hpp"

#include "Zinet/Core/ZtLogger.hpp"

#include <vulkan/vulkan.h>

#include <vk_mem_alloc.h>

namespace zt::vulkan_renderer
{
	class VMA;

	class ZINET_VULKAN_RENDERER_API Buffer : public VulkanObject<VkBuffer>
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("VRBuffer");

	public:

		Buffer(HandleType newObjectHandle)
			: VulkanObject(newObjectHandle) {}

		Buffer() noexcept = delete;
		Buffer(const Buffer& other) noexcept = delete;
		Buffer(Buffer&& other) noexcept = default;
		~Buffer() noexcept = default;

		Buffer& operator = (const Buffer& other) noexcept = delete;
		Buffer& operator = (Buffer&& other) noexcept = default;

		template<class ContainerT>
		static VkBufferCreateInfo GetVertexBufferCreateInfo(const ContainerT& vertices) noexcept;

		template<class ContainerT>
		static VkBufferCreateInfo GetIndexBufferCreateInfo(const ContainerT& vertices) noexcept;

		bool createBuffer(const VkBufferCreateInfo& createInfo, const VMA& vma) noexcept;

		template<class ContainerT>
		bool fill(const ContainerT& contiguousContainer, const VMA& vma) noexcept;

		template<class ContainerT>
		bool getData(ContainerT& contiguousContainer, const VMA& vma) const noexcept;

		void destroy(const VMA& vma) noexcept;

		std::uint32_t getSize() const noexcept { return size; }

	protected:

		VmaAllocation allocation{};
		std::uint32_t size{};

	};

	template<class ContainerT>
	VkBufferCreateInfo Buffer::GetVertexBufferCreateInfo(const ContainerT& vertices) noexcept
	{
		VkBufferCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		createInfo.size = sizeof(typename ContainerT::value_type) * vertices.size();
		createInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
		createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		return createInfo;
	}

	template<class ContainerT>
	VkBufferCreateInfo Buffer::GetIndexBufferCreateInfo(const ContainerT& indices) noexcept
	{
		VkBufferCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		createInfo.size = sizeof(typename ContainerT::value_type) * indices.size();
		createInfo.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
		createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		return createInfo;
	}

	template<class ContainerT>
	bool Buffer::fill(const ContainerT& contiguousContainer, const VMA& vma) noexcept
	{
		if (!isValid())
			return false;

		const auto containerSize = sizeof(typename ContainerT::value_type) * contiguousContainer.size();
		if (containerSize != size)
			return false;

		void* mappedData{};
		const auto mapResult = vmaMapMemory(vma.get(), allocation, &mappedData);

		if (mapResult == VK_SUCCESS)
		{
			std::memcpy(mappedData, contiguousContainer.data(), size);
			vmaUnmapMemory(vma.get(), allocation);
			return true;
		}
		else
		{
			Logger->error("Couldn't fill buffer, result: {}", static_cast<std::int32_t>(mapResult));
			return false;
		}
	}

	template<class ContainerT>
	bool Buffer::getData(ContainerT& contiguousContainer, const VMA& vma) const noexcept
	{
		if (!isValid())
			return false;

		const auto containerSize = sizeof(typename ContainerT::value_type) * contiguousContainer.size();
		if (containerSize != size)
			return false;

		void* mappedData{};
		const auto mapResult = vmaMapMemory(vma.get(), allocation, &mappedData);

		if (mapResult == VK_SUCCESS)
		{
			std::memcpy(contiguousContainer.data(), mappedData, size);
			vmaUnmapMemory(vma.get(), allocation);
			return true;
		}
		else
		{
			Logger->error("Couldn't get buffer data, result: {}", static_cast<std::int32_t>(mapResult));
			return false;
		}
	}
}