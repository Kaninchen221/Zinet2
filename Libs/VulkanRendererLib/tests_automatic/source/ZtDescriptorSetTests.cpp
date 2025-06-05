#pragma once

#include "Zinet/VulkanRenderer/Tests/ZtTestUtils.hpp"

#include "Zinet/VulkanRenderer/ZtDescriptorSet.hpp"
#include "Zinet/VulkanRenderer/ZtPhysicalDevice.hpp"
#include "Zinet/VulkanRenderer/ZtInstance.hpp"
#include "Zinet/VulkanRenderer/ZtDebugUtilsMessenger.hpp"
#include "Zinet/VulkanRenderer/ZtDescriptorSetLayout.hpp"
#include "Zinet/VulkanRenderer/ZtDescriptorPool.hpp"
#include "Zinet/VulkanRenderer/ZtBuffer.hpp"

#include <gtest/gtest.h>

#include <vulkan/vulkan.h>

#include "Zinet/Window/ZtGLFW.hpp"
#include "Zinet/Window/ZtWindow.hpp"

#include <type_traits>

namespace zt::vulkan_renderer::tests
{
	class DescriptorSetTests : public ::testing::Test
	{
	protected:

		DescriptorSetTests()
		{
		}

		~DescriptorSetTests() override
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

			ASSERT_TRUE(vma.create(device, physicalDevice, instance));

			const Vector2f position{};
			const auto uniformBufferCreateInfo = Buffer::GetUniformBufferCreateInfo(position);
			uniformBuffer.createBuffer(uniformBufferCreateInfo, vma);

			const auto descriptorPoolSize = DescriptorPool::GetDefaultDescriptorPoolSize();
			std::vector<VkDescriptorPoolSize> descriptorPoolSizes{ descriptorPoolSize };
			const auto descriptorPoolCreateInfo = DescriptorPool::GetDefaultCreateInfo(descriptorPoolSizes);
			ASSERT_TRUE(descriptorPool.create(device, descriptorPoolCreateInfo));

			const auto layoutBinding = DescriptorSetLayout::GetDefaultLayoutBinding();
			const DescriptorSetLayout::Bindings bindings{ layoutBinding };

			const auto createInfo = DescriptorSetLayout::GetDefaultCreateInfo(bindings);
			ASSERT_TRUE(descriptorSetLayout.create(createInfo, device));
			ASSERT_TRUE(descriptorSetLayout.isValid());

			const std::vector descriptorSetLayouts{ descriptorSetLayout.get() };
			const auto allocateInfo = DescriptorSet::GetDefaultAllocateInfo(descriptorPool, descriptorSetLayouts);
			ASSERT_TRUE(descriptorSet.create(device, allocateInfo));
			ASSERT_TRUE(descriptorSet.isValid());

			const VkDescriptorBufferInfo descriptorBufferInfo = DescriptorSet::GetBufferInfo(uniformBuffer);
			auto writeDescriptorSet = DescriptorSet::GetDefaultWriteDescriptorSet();
			writeDescriptorSet.pBufferInfo = &descriptorBufferInfo;
			writeDescriptorSet.dstSet = descriptorSet.get();
			descriptorSet.update(device, writeDescriptorSet);
		}

		void TearDown() override
		{
			descriptorSetLayout.destroy(device);
			ASSERT_FALSE(descriptorSetLayout.isValid());

			descriptorPool.destroy(device);
			ASSERT_FALSE(descriptorPool.isValid());

			descriptorSet.invalidate();
			ASSERT_FALSE(descriptorSet.isValid());

			uniformBuffer.destroy(vma);
			ASSERT_FALSE(uniformBuffer.isValid());

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

			wd::GLFW::Deinit();
		}

		Instance instance{ nullptr };
		DebugUtilsMessenger debugUtilsMessenger{ nullptr };
		PhysicalDevice physicalDevice{ nullptr };
		Device device{ nullptr };
		VMA vma{ nullptr };
		Buffer uniformBuffer{ nullptr };
		DescriptorPool descriptorPool{ nullptr };
		DescriptorSetLayout descriptorSetLayout{ nullptr };
		DescriptorSet descriptorSet{ nullptr };

		static_assert(VulkanObjectDecoratorStaticTest<DescriptorSet, VkDescriptorSet>());
	};

	TEST_F(DescriptorSetTests, PassTest)
	{

	}
}