#pragma once

#include "Zinet/VulkanRenderer/ZtBuffer.hpp"
#include "Zinet/VulkanRenderer/ZtVertex.hpp"
#include "Zinet/VulkanRenderer/ZtInstance.hpp"
#include "Zinet/VulkanRenderer/ZtDebugUtilsMessenger.hpp"
#include "Zinet/VulkanRenderer/ZtPhysicalDevice.hpp"
#include "Zinet/VulkanRenderer/ZtDevice.hpp"
#include "Zinet/VulkanRenderer/ZtVMA.hpp"

#include <gtest/gtest.h>

#include <vulkan/vulkan.h>

#include <type_traits>

namespace zt::vulkan_renderer::tests
{
	class BufferTests : public ::testing::Test
	{
	protected:

		BufferTests()
		{
		}

		~BufferTests() override
		{
		}

		void SetUp() override
		{
			instance.setEnableValidationLayers(true);
			ASSERT_TRUE(instance.create());

			ASSERT_TRUE(debugUtilsMessenger.create(instance));

			auto physicalDevices = instance.getPhysicalDevices();
			physicalDevice = PhysicalDevice::TakeBestPhysicalDevice(physicalDevices);
			ASSERT_TRUE(physicalDevice.isValid());

			ASSERT_TRUE(device.create(physicalDevice, Surface{ nullptr }));

			ASSERT_TRUE(vma.create(device, physicalDevice, instance));
		}

		void TearDown() override
		{
			buffer.destroy(vma);
			ASSERT_FALSE(buffer.isValid());

			vma.destroy();
			ASSERT_FALSE(vma.isValid());

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
		VMA vma{ nullptr };
		Buffer buffer{ nullptr };

		static_assert(std::is_base_of_v<VulkanObject<VkBuffer>, Buffer>);

		static_assert(!std::is_default_constructible_v<Buffer>);
		static_assert(!std::is_copy_constructible_v<Buffer>);
		static_assert(!std::is_copy_assignable_v<Buffer>);
		static_assert(std::is_move_constructible_v<Buffer>);
		static_assert(std::is_move_assignable_v<Buffer>);
		static_assert(std::is_destructible_v<Buffer>);
	};

	TEST_F(BufferTests, PassTest)
	{
		const std::vector<Vertex> vertices{ 
			Vertex{ { 23.f, 212.f, 1.f }, { 0.4f, 0.4f, 0.1f } },
			Vertex{ { 51, 21.f, 1.2f }, { 0.1f, 0.2f, 0.4f } },
			Vertex{ { 65.f, 23.f, 111.f }, { 0.7f, 0.4f, 0.8f } }
		};

		buffer.createVertexBuffer(vertices, vma);
		ASSERT_EQ(sizeof(Vertex) * vertices.size(), buffer.getSize());

		buffer.fill(vertices, vma);

		std::vector<Vertex> actualVertices(vertices.size(), Vertex{});
		buffer.getData(actualVertices, vma);

		ASSERT_TRUE(core::CompareContainers(vertices, actualVertices));
	}

}