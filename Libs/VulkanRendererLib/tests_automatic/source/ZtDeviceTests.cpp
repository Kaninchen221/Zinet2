#pragma once

#include "Zinet/VulkanRenderer/Tests/ZtTestUtils.hpp"

#include "Zinet/VulkanRenderer/ZtInstance.hpp"
#include "Zinet/VulkanRenderer/ZtDebugUtilsMessenger.hpp"
#include "Zinet/VulkanRenderer/ZtPhysicalDevice.hpp"
#include "Zinet/VulkanRenderer/ZtDevice.hpp"
#include "Zinet/VulkanRenderer/ZtSwapChain.hpp"
#include "Zinet/VulkanRenderer/ZtBuffer.hpp"

#include "Zinet/Core/ZtPaths.hpp"
#include "Zinet/Core/ZtUtils.hpp"

#include <gtest/gtest.h>

#include <vulkan/vulkan.h>

#include "Zinet/Window/ZtGLFW.hpp"
#include "Zinet/Window/ZtWindow.hpp"

#include <type_traits>

namespace zt::vulkan_renderer::tests
{
	class DeviceTests : public ::testing::Test
	{
	protected:

		void SetUp() override
		{
			wd::GLFW::Init();

			window.create(2, 2);

			instance.setEnableValidationLayers(true);
			ASSERT_TRUE(instance.create());
			
			ASSERT_TRUE(debugUtilsMessenger.create(instance));

			auto physicalDevices = instance.getPhysicalDevices();
			physicalDevice = PhysicalDevice::TakeBestPhysicalDevice(physicalDevices);
			invalidateAll(physicalDevices);
			ASSERT_TRUE(physicalDevice.isValid());

			ASSERT_TRUE(surface.create(instance, window));

			ASSERT_TRUE(device.create(instance, physicalDevice, surface));
		}

		void TearDown() override
		{
			surface.destroy(instance);
			ASSERT_FALSE(surface.isValid());

			device.destroy();
			ASSERT_FALSE(device.isValid());

			physicalDevice.invalidate();
			ASSERT_FALSE(physicalDevice.isValid());

			debugUtilsMessenger.destroy(instance);
			ASSERT_FALSE(debugUtilsMessenger.isValid());

			instance.destroy();
			ASSERT_FALSE(instance.isValid());

			window.destroyWindow();

			wd::GLFW::Deinit();
		}

		Instance instance{ nullptr };
		DebugUtilsMessenger debugUtilsMessenger{ nullptr };
		PhysicalDevice physicalDevice{ nullptr };
		wd::Window window;
		Surface surface{ nullptr };
		Device device{ nullptr };

		static_assert(VulkanObjectDecoratorStaticTest<Device, VkDevice>());
	};

	TEST_F(DeviceTests, PassTest)
	{}

	TEST_F(DeviceTests, GetQueueFamilyIndexTest)
	{
		const std::int32_t queueFamilyIndex = device.getQueueFamilyIndex();
		ASSERT_NE(queueFamilyIndex, Device::InvalidIndex);
	}

	TEST_F(DeviceTests, GetQueueTest)
	{
		Queue presentQueue = device.getQueue();
		ASSERT_TRUE(presentQueue.isValid());

		presentQueue.invalidate();
	}

	TEST_F(DeviceTests, SetDebugName)
	{
		VMA vma{ nullptr };
		vma.create(device, physicalDevice, instance);
		ASSERT_TRUE(vma.isValid());

		Buffer buffer{ nullptr };
		int i = 50;
		auto bufferCreateInfo = Buffer::GetUniformBufferCreateInfo(i);
		buffer.createBuffer(bufferCreateInfo, vma);
		ASSERT_TRUE(buffer.isValid());

		std::string_view bufferDebugName = "BufferDebugName";
		ASSERT_TRUE(device.setDebugName(buffer, bufferDebugName, VK_OBJECT_TYPE_BUFFER));

		buffer.destroy(vma);
		vma.destroy();
	}
}