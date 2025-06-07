#pragma once

#include "Zinet/VulkanRenderer/Tests/ZtTestUtils.hpp"

#include "Zinet/VulkanRenderer/ZtDescriptorSetLayout.hpp"
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
	class DescriptorSetLayoutTests : public ::testing::Test
	{
	protected:

		DescriptorSetLayoutTests()
		{
		}

		~DescriptorSetLayoutTests() override
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

			const auto layoutBinding = DescriptorSetLayout::GetDefaultUniformLayoutBinding();
			const DescriptorSetLayout::Bindings bindings{ layoutBinding };
			const auto createInfo = DescriptorSetLayout::GetDefaultCreateInfo(bindings);

			ASSERT_TRUE(descriptorSetLayout.create(createInfo, device));
			ASSERT_TRUE(descriptorSetLayout.isValid());
		}

		void TearDown() override
		{
			descriptorSetLayout.destroy(device);
			ASSERT_FALSE(descriptorSetLayout.isValid());

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
		DescriptorSetLayout descriptorSetLayout{ nullptr };

		static_assert(VulkanObjectDecoratorStaticTest<DescriptorSetLayout, VkDescriptorSetLayout>());
	};

	TEST_F(DescriptorSetLayoutTests, PassTest)
	{
	}
}