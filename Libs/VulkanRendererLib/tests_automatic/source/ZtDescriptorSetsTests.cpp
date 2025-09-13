#pragma once

#include "Zinet/VulkanRenderer/Tests/ZtTestUtils.hpp"

#include "Zinet/VulkanRenderer/ZtDescriptorSets.hpp"
#include "Zinet/VulkanRenderer/ZtPhysicalDevice.hpp"
#include "Zinet/VulkanRenderer/ZtInstance.hpp"
#include "Zinet/VulkanRenderer/ZtDebugUtilsMessenger.hpp"
#include "Zinet/VulkanRenderer/ZtDescriptorSetLayout.hpp"
#include "Zinet/VulkanRenderer/ZtDescriptorPool.hpp"
#include "Zinet/VulkanRenderer/ZtBuffer.hpp"
#include "Zinet/VulkanRenderer/ZtDrawInfo.hpp"

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

			ASSERT_TRUE(device.create(instance, physicalDevice, Surface{ nullptr }));

			ASSERT_TRUE(vma.create(device, physicalDevice, instance));

			const Vector2f position{};
			const auto uniformBufferCreateInfo = Buffer::GetUniformBufferCreateInfo(position);
			uniformBuffer.createBuffer(uniformBufferCreateInfo, vma);

			const auto descriptorPoolSizes = DescriptorPool::GetDefaultPoolSizes();
			const auto descriptorPoolCreateInfo = DescriptorPool::GetDefaultCreateInfo(descriptorPoolSizes);
			ASSERT_TRUE(descriptorPool.create(device, descriptorPoolCreateInfo));

			const auto layoutBinding = DescriptorSetLayout::GetDefaultUniformLayoutBinding();
			const DescriptorSetLayout::Bindings bindings{ layoutBinding };

			const auto createInfo = DescriptorSetLayout::GetDefaultCreateInfo(bindings);
			ASSERT_TRUE(descriptorSetLayout.create(createInfo, device));
			ASSERT_TRUE(descriptorSetLayout.isValid());

			ASSERT_EQ(descriptorSets.getCount(), 1);

			const std::vector descriptorSetLayouts{ descriptorSetLayout.get() };
			const auto allocateInfo = DescriptorSets::GetDefaultAllocateInfo(descriptorPool, descriptorSetLayouts);
			ASSERT_TRUE(descriptorSets.create(device, allocateInfo));
			ASSERT_EQ(descriptorSets.getCount(), descriptorSetLayouts.size());
			ASSERT_TRUE(descriptorSets.isValid());
			ASSERT_TRUE(descriptorSets.get());
			ASSERT_TRUE(descriptorSets.data());
			ASSERT_TRUE(descriptorSets.operator bool());

			const VkDescriptorBufferInfo descriptorBufferInfo = DescriptorInfo::GetBufferInfo(uniformBuffer);

			DescriptorSetsUpdateData updateData;
			updateData.descriptorBuffersInfos.push_back(descriptorBufferInfo);

			auto writeDescriptorSet = DescriptorInfo::GetDefaultWriteDescriptorSet();
			writeDescriptorSet.pBufferInfo = &descriptorBufferInfo;
			writeDescriptorSet.dstSet = descriptorSets.get();

			updateData.writeDescriptorSets.push_back(writeDescriptorSet);

			descriptorSets.update(device, updateData);
		}

		void TearDown() override
		{
			descriptorSetLayout.destroy(device);
			ASSERT_FALSE(descriptorSetLayout.isValid());

			descriptorPool.destroy(device);
			ASSERT_FALSE(descriptorPool.isValid());

			descriptorSets.invalidate();
			ASSERT_FALSE(descriptorSets.isValid());

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
		DescriptorSets descriptorSets;
	};

	TEST_F(DescriptorSetTests, PassTest)
	{

	}
}