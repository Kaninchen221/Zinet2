#pragma once

#include "Zinet/VulkanRenderer/Tests/ZtTestUtils.hpp"

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

#include "Zinet/Core/ZtImage.hpp"
#include "Zinet/Core/ZtPaths.hpp"

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
			invalidateAll(physicalDevices);
			ASSERT_TRUE(physicalDevice.isValid());

			ASSERT_TRUE(device.create(instance, physicalDevice, Surface{ nullptr }));

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
		Buffer buffer{ nullptr };

		static_assert(VulkanObjectDecoratorStaticTest<Buffer, VkBuffer>());
	};

	TEST_F(BufferTests, VertexBufferTest)
	{
		using Vertices = std::vector<Vertex>;
		const Vertices vertices
		{ 
			Vertex{ { 23.f, 212.f, 1.f }, { 0.4f, 0.4f, 0.1f, 1.f } },
			Vertex{ { 51, 21.f, 1.2f }, { 0.1f, 0.2f, 0.4f, 1.f } },
			Vertex{ { 65.f, 23.f, 111.f }, { 0.7f, 0.4f, 0.8f, 1.f } }
		};

		const auto createInfo = Buffer::GetVertexBufferCreateInfo(vertices);
		ASSERT_TRUE(buffer.createBuffer(createInfo, vma));
		ASSERT_EQ(sizeof(Vertices::value_type) * vertices.size(), buffer.getSize());
		ASSERT_TRUE(buffer.getAllocation());

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
		ASSERT_TRUE(buffer.getAllocation());

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
		ASSERT_TRUE(buffer.getAllocation());

		ASSERT_TRUE(buffer.fillWithObject(uniformBufferData, vma));
		
		DataT actualData{};
		ASSERT_TRUE(buffer.getDataToObject(actualData, vma));
		
		ASSERT_EQ(uniformBufferData, actualData);
	}

	TEST_F(BufferTests, ImageBufferTest)
	{
		core::Image sourceImage;

		const std::filesystem::path testFolderPath = core::Paths::CurrentProjectRootPath() / "test_files";
		const std::filesystem::path imagePath = testFolderPath / "image.png";
		ASSERT_TRUE(sourceImage.loadFromFile(imagePath, 4));

		const auto createInfo = Buffer::GetImageBufferCreateInfo(sourceImage);
		ASSERT_TRUE(buffer.createBuffer(createInfo, vma));
	}

}