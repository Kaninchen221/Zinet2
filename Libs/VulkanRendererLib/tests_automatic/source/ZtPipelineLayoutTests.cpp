#pragma once

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
			ASSERT_TRUE(physicalDevice.isValid());

			ASSERT_TRUE(device.create(physicalDevice, Surface{ nullptr }));

			ASSERT_TRUE(pipelineLayout.create(device));
			ASSERT_TRUE(pipelineLayout.isValid());
		}

		void TearDown() override
		{
			pipelineLayout.destroy(device);
			ASSERT_FALSE(pipelineLayout.isValid());

			device.destroy();
			ASSERT_FALSE(device.isValid());

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

		static_assert(std::is_base_of_v<VulkanObject<VkPipelineLayout>, PipelineLayout>);

		static_assert(std::is_constructible_v<PipelineLayout, VkPipelineLayout>);
		static_assert(!std::is_default_constructible_v<PipelineLayout>);
		static_assert(!std::is_copy_constructible_v<PipelineLayout>);
		static_assert(!std::is_copy_assignable_v<PipelineLayout>);
		static_assert(std::is_move_constructible_v<PipelineLayout>);
		static_assert(std::is_move_assignable_v<PipelineLayout>);
		static_assert(std::is_destructible_v<PipelineLayout>);
	};

	TEST_F(PipelineLayoutTests, PassTest)
	{}
}