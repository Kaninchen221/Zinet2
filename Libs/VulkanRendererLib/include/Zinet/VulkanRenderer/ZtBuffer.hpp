#pragma once

#include "Zinet/VulkanRenderer/ZtVulkanRendererConfig.hpp"
#include "Zinet/VulkanRenderer/ZtVulkanObject.hpp"
#include "Zinet/VulkanRenderer/ZtVertex.hpp"
#include "Zinet/VulkanRenderer/ZtVMA.hpp"

#include "Zinet/Core/ZtLogger.hpp"
#include "Zinet/Core/ZtConcepts.hpp"

#include <vulkan/vulkan.h>

#include <vk_mem_alloc.h>

namespace zt::core
{
	class Image;
}

namespace zt::vulkan_renderer
{
	class VMA;

	class  Buffer : public VulkanObject<VkBuffer>
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("VRBuffer");

	public:

		Buffer(HandleType newObjectHandle) noexcept
			: VulkanObject(newObjectHandle) {}

		Buffer() noexcept = delete;
		Buffer(const Buffer& other) noexcept = delete;
		Buffer(Buffer&& other) noexcept = default;
		~Buffer() noexcept = default;

		Buffer& operator = (const Buffer& other) noexcept = delete;
		Buffer& operator = (Buffer&& other) noexcept = default;

		template<core::STDContainer ContainerT>
		static VkBufferCreateInfo GetVertexBufferCreateInfo(const ContainerT& vertices) noexcept;

		template<core::STDContainer ContainerT>
		static VkBufferCreateInfo GetIndexBufferCreateInfo(const ContainerT& indices) noexcept;

		template<core::NotSTDContainer ObjectT>
		static VkBufferCreateInfo GetUniformBufferCreateInfo(const ObjectT& data) noexcept;

		static VkBufferCreateInfo GetImageBufferCreateInfo(const core::Image& image) noexcept;

		template<core::STDContainer ContainerT>
		static VkBufferCreateInfo GetStorageBufferCreateInfo(const ContainerT& container) noexcept;

		bool createBuffer(const VMA& vma, const VkBufferCreateInfo& createInfo);

		template<core::STDContainer ContainerT>
		bool fillWithSTDContainer(const VMA& vma, const ContainerT& contiguousContainer, size_t bytesDstOffset = 0);

		template<core::NotSTDContainer ObjectT>
		bool fillWithObject(const VMA& vma, const ObjectT& object, size_t bytesDstOffset = 0);

		bool fillWithImage(const VMA& vma, const core::Image& image, size_t bytesDstOffset = 0);

		template<core::STDContainer ContainerT>
		bool getDataToSTDContainer(const VMA& vma, ContainerT& contiguousContainer, size_t bytesSrcOffset = 0) const;

		template<core::NotSTDContainer ObjectT>
		bool getDataToObject(const VMA& vma, ObjectT& object, size_t bytesSrcOffset = 0) const;

		void destroy(const VMA& vma) noexcept;

		std::uint32_t getSize() const noexcept { return size; }

		VmaAllocation getAllocation() const noexcept { return allocation; }

	protected:

		VkResult fillWithData(const VMA& vma, const void* src, size_t srcSize, size_t bytesDstOffset = 0) const;

		VkResult getData(const VMA& vma, void* dst, size_t dstSize, size_t bytesDstOffset = 0) const noexcept;

		VmaAllocation allocation{};
		std::uint32_t size{};

	};

	template<core::STDContainer ContainerT>
	VkBufferCreateInfo Buffer::GetVertexBufferCreateInfo(const ContainerT& vertices) noexcept
	{
		return VkBufferCreateInfo
		{
			.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
			.size = sizeof(typename ContainerT::value_type) * vertices.size(),
			.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			.sharingMode = VK_SHARING_MODE_EXCLUSIVE
		};
	}

	template<core::STDContainer ContainerT>
	VkBufferCreateInfo Buffer::GetIndexBufferCreateInfo(const ContainerT& indices) noexcept
	{
		return VkBufferCreateInfo
		{
			.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
			.size = sizeof(typename ContainerT::value_type) * indices.size(),
			.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			.sharingMode = VK_SHARING_MODE_EXCLUSIVE
		};
	}

	template<core::NotSTDContainer ObjectT>
	VkBufferCreateInfo Buffer::GetUniformBufferCreateInfo([[maybe_unused]] const ObjectT& data) noexcept
	{
		return VkBufferCreateInfo
		{
			.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
			.size = sizeof(ObjectT),
			.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			.sharingMode = VK_SHARING_MODE_EXCLUSIVE
		};
	}

	template<core::STDContainer ContainerT>
	VkBufferCreateInfo Buffer::GetStorageBufferCreateInfo(const ContainerT& container) noexcept
	{
		return VkBufferCreateInfo
		{
			.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
			.size = sizeof(typename ContainerT::value_type) * container.size(),
			.usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			.sharingMode = VK_SHARING_MODE_EXCLUSIVE
		};
	}

	template<core::STDContainer ContainerT>
	bool Buffer::fillWithSTDContainer(const VMA& vma, const ContainerT& stdContainer, size_t bytesOffset)
	{
		if (!isValid())
			return false;

		const auto containerSize = sizeof(typename ContainerT::value_type) * stdContainer.size();
		const auto result = fillWithData(vma, stdContainer.data(), containerSize, bytesOffset);

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

	template<core::NotSTDContainer ObjectT>
	bool Buffer::fillWithObject(const VMA& vma, const ObjectT& object, size_t bytesOffset)
	{
		if (!isValid())
			return false;

		const auto result = fillWithData(vma, &object, sizeof(ObjectT), bytesOffset);

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

	template<core::STDContainer ContainerT>
	bool Buffer::getDataToSTDContainer(const VMA& vma, ContainerT& stdContainer, size_t bytesSrcOffset) const
	{
		if (!isValid())
			return false;

		const auto containerSize = sizeof(typename ContainerT::value_type) * stdContainer.size();
		const auto result = getData(vma, stdContainer.data(), containerSize, bytesSrcOffset);

		if (result == VK_SUCCESS)
		{
			return true;
		}
		else
		{
			Logger->error("Couldn't get buffer data, result: {}", static_cast<std::int32_t>(result));
			return false;
		}
	}

	template<core::NotSTDContainer ObjectT>
	bool Buffer::getDataToObject(const VMA& vma, ObjectT& object, size_t bytesSrcOffset) const
	{
		if (!isValid())
			return false;

		const auto result = getData(vma, &object, sizeof(object), bytesSrcOffset);

		if (result == VK_SUCCESS)
		{
			return true;
		}
		else
		{
			Logger->error("Couldn't get buffer data, result: {}", static_cast<std::int32_t>(result));
			return false;
		}
	}
}