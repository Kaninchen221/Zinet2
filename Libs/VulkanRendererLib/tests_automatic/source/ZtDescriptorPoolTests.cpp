#pragma once

#include "Zinet/VulkanRenderer/ZtDescriptorPool.hpp"
#include "Zinet/VulkanRenderer/ZtPhysicalDevice.hpp"
#include "Zinet/VulkanRenderer/ZtInstance.hpp"
#include "Zinet/VulkanRenderer/ZtDebugUtilsMessenger.hpp"

#include <gtest/gtest.h>

#include <vulkan/vulkan.h>

#include "Zinet/Window/ZtGLFW.hpp"
#include "Zinet/Window/ZtWindow.hpp"

#include <type_traits>

namespace zt::vulkan_renderer::tests
{
	class DescriptorPoolTests : public ::testing::Test
	{
	protected:

		DescriptorPoolTests()
		{
		}

		~DescriptorPoolTests() override
		{
		}

		void SetUp() override
		{
			wd::GLFW::Init();

			instance.setEnableValidationLayers(true);
			ASSERT_TRUE(instance.create());

			ASSERT_TRUE(debugUtilsMessenger.create(instance));

			auto physicalDevices = instance.getPhysicalDevices();
			physicalDevice = PhysicalDevice::TakeBestPhysicalDevice(physicalDevices);
			invalidateAll(physicalDevices);
			ASSERT_TRUE(physicalDevice.isValid());

			ASSERT_TRUE(device.create(physicalDevice, Surface{ nullptr }));

			const auto descriptorPoolSize = DescriptorPool::GetDefaultDescriptorPoolSize();
			std::vector<VkDescriptorPoolSize> descriptorPoolSizes{ descriptorPoolSize };
			const auto descriptorPoolCreateInfo = DescriptorPool::GetDefaultCreateInfo(descriptorPoolSizes);
			
			ASSERT_TRUE(descriptorPool.create(device, descriptorPoolCreateInfo));
			ASSERT_TRUE(descriptorPool.isValid());
		}

		void TearDown() override
		{
			descriptorPool.destroy(device);
			ASSERT_FALSE(descriptorPool.isValid());

			device.destroy();
			ASSERT_FALSE(device.isValid());

			physicalDevice.invalidate();
			ASSERT_FALSE(physicalDevice.isValid());

			debugUtilsMessenger.destroy(instance);
			ASSERT_FALSE(debugUtilsMessenger.isValid());

			instance.destroy();
			ASSERT_FALSE(instance.isValid());

			wd::GLFW::Deinit();
		}

		Instance instance{ nullptr };
		DebugUtilsMessenger debugUtilsMessenger{ nullptr };
		PhysicalDevice physicalDevice{ nullptr };
		Device device{ nullptr };
		DescriptorPool descriptorPool{ nullptr };

		static_assert(std::is_base_of_v<VulkanObject<VkDescriptorPool>, DescriptorPool>);

		static_assert(!std::is_default_constructible_v<DescriptorPool>);
		static_assert(!std::is_copy_constructible_v<DescriptorPool>);
		static_assert(!std::is_copy_assignable_v<DescriptorPool>);
		static_assert(std::is_move_constructible_v<DescriptorPool>);
		static_assert(std::is_move_assignable_v<DescriptorPool>);
		static_assert(std::is_destructible_v<DescriptorPool>);
	};

	TEST_F(DescriptorPoolTests, PassTest)
	{

	}
}