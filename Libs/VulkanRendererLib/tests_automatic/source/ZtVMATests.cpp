#pragma once

#include "Zinet/VulkanRenderer/ZtVMA.hpp"
#include "Zinet/VulkanRenderer/ZtDevice.hpp"
#include "Zinet/VulkanRenderer/ZtPhysicalDevice.hpp"
#include "Zinet/VulkanRenderer/ZtDebugUtilsMessenger.hpp"
#include "Zinet/VulkanRenderer/ZtInstance.hpp"

#include <gtest/gtest.h>

#include <type_traits>

#define VMA_IMPLEMENTATION
#include <vk_mem_alloc.h>

namespace zt::vulkan_renderer::tests
{
	class VMATests : public ::testing::Test
	{
	protected:

		void SetUp() override
		{
			instance.setEnableValidationLayers(true);
			ASSERT_TRUE(instance.create());

			ASSERT_TRUE(debugUtilsMessenger.create(instance));

			auto physicalDevices = instance.getPhysicalDevices();
			physicalDevice = PhysicalDevice::TakeBestPhysicalDevice(physicalDevices);
			invalidateAll(physicalDevices);
			ASSERT_TRUE(physicalDevice.isValid());

			ASSERT_TRUE(device.create(physicalDevice, Surface{ nullptr }));

			ASSERT_TRUE(vma.create(device, physicalDevice, instance));
			ASSERT_TRUE(vma.isValid());
		}

		void TearDown() override
		{
			vma.destroy();
			ASSERT_FALSE(vma.isValid());

			device.destroy();
			ASSERT_FALSE(device.isValid());

			physicalDevice.invalidate();
			ASSERT_FALSE(physicalDevice.isValid());

			debugUtilsMessenger.destroy(instance);
			ASSERT_FALSE(debugUtilsMessenger.isValid());

			instance.destroy();
			ASSERT_FALSE(instance.isValid());
		}

		Instance instance{ nullptr };
		DebugUtilsMessenger debugUtilsMessenger{ nullptr };
		PhysicalDevice physicalDevice{ nullptr };
		Device device{ nullptr };
		VMA vma{ nullptr };

		/// VmaAllocator is not a vulkan object but VMA project is using similar style
		static_assert(std::is_base_of_v<VulkanObject<VmaAllocator>, VMA>);

		static_assert(std::is_constructible_v<VMA, VmaAllocator>);
		static_assert(!std::is_default_constructible_v<VMA>);
		static_assert(!std::is_copy_constructible_v<VMA>);
		static_assert(!std::is_copy_assignable_v<VMA>);
		static_assert(std::is_move_constructible_v<VMA>);
		static_assert(std::is_move_assignable_v<VMA>);
		static_assert(std::is_destructible_v<VMA>);
	};

	TEST_F(VMATests, PassTest)
	{
		VkBufferCreateInfo bufferInfo = { VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO };
		bufferInfo.size = 65536;
		bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;

		VmaAllocationCreateInfo allocInfo = {};
		allocInfo.usage = VMA_MEMORY_USAGE_AUTO;

		VkBuffer buffer{};
		VmaAllocation allocation{};
		vmaCreateBuffer(vma.get(), &bufferInfo, &allocInfo, &buffer, &allocation, nullptr);

		ASSERT_NE(buffer, nullptr);
		ASSERT_NE(allocation, nullptr);

		vmaDestroyBuffer(vma.get(), buffer, allocation);
	}
}