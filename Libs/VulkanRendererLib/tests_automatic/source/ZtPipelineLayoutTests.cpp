#pragma once

#include "Zinet/VulkanRenderer/Tests/ZtTestUtils.hpp"

#include <gtest/gtest.h>

#include "Zinet/VulkanRenderer/ZtPipelineLayout.hpp"
#include "Zinet/VulkanRenderer/ZtDevice.hpp"
#include "Zinet/VulkanRenderer/ZtSurface.hpp"
#include "Zinet/VulkanRenderer/ZtPhysicalDevice.hpp"
#include "Zinet/VulkanRenderer/ZtDebugUtilsMessenger.hpp"
#include "Zinet/VulkanRenderer/ZtInstance.hpp"

namespace zt::vulkan_renderer::tests
{
	class PipelineLayoutTests : public ::testing::Test
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

			const auto pipelineLayoutCreateInfo = PipelineLayout::GetDefaultCreateInfo({});
			ASSERT_TRUE(pipelineLayout.create(device, pipelineLayoutCreateInfo));
			ASSERT_TRUE(pipelineLayout.isValid());
		}

		void TearDown() override
		{
			pipelineLayout.destroy(device);
			ASSERT_FALSE(pipelineLayout.isValid());

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
		PipelineLayout pipelineLayout{ nullptr };

		static_assert(VulkanObjectDecoratorStaticTest<PipelineLayout, VkPipelineLayout>());
	};

	TEST_F(PipelineLayoutTests, PassTest)
	{}
}