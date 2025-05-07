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

	TEST_F(BufferTests, VertexBufferTest)
	{
		using Vertices = std::vector<Vertex>;
		const Vertices vertices
		{ 
			Vertex{ { 23.f, 212.f, 1.f }, { 0.4f, 0.4f, 0.1f } },
			Vertex{ { 51, 21.f, 1.2f }, { 0.1f, 0.2f, 0.4f } },
			Vertex{ { 65.f, 23.f, 111.f }, { 0.7f, 0.4f, 0.8f } }
		};

		const auto createInfo = Buffer::GetVertexBufferCreateInfo(vertices);
		ASSERT_TRUE(buffer.createBuffer(createInfo, vma));
		ASSERT_EQ(sizeof(Vertices::value_type) * vertices.size(), buffer.getSize());

		ASSERT_TRUE(buffer.fillWithSTDContainer(vertices, vma));

		Vertices actualVertices(vertices.size(), Vertices::value_type{});
		ASSERT_TRUE(buffer.getDataToSTDContainer(actualVertices, vma));

		ASSERT_TRUE(core::CompareContainers(vertices, actualVertices));
	}

	TEST_F(BufferTests, IndicesBufferTest)
	{
		using Indices = std::vector<uint16_t>;
		const Indices indices =
		{
			0, 1, 2, 
			2, 3, 0
		};

		const auto createInfo = Buffer::GetIndexBufferCreateInfo(indices);
		ASSERT_TRUE(buffer.createBuffer(createInfo, vma));
		ASSERT_EQ(sizeof(Indices::value_type) * indices.size(), buffer.getSize());

		ASSERT_TRUE(buffer.fillWithSTDContainer(indices, vma));

		Indices actualIndices(indices.size(), Indices::value_type{});
		ASSERT_TRUE(buffer.getDataToSTDContainer(actualIndices, vma));

		ASSERT_TRUE(core::CompareContainers(indices, actualIndices));
	}

	TEST_F(BufferTests, UniformBufferTest)
	{
		using DataT = Vector2f;
		const DataT uniformBufferData{ 123.7842f, 0.8823f };

		const auto createInfo = Buffer::GetUniformBufferCreateInfo(uniformBufferData);
		ASSERT_TRUE(buffer.createBuffer(createInfo, vma));
		ASSERT_EQ(sizeof(DataT), buffer.getSize());

		ASSERT_TRUE(buffer.fillWithObject(uniformBufferData, vma));
		
		DataT actualData{};
		ASSERT_TRUE(buffer.getDataToObject(actualData, vma));
		
		ASSERT_EQ(uniformBufferData, actualData);
	}

}