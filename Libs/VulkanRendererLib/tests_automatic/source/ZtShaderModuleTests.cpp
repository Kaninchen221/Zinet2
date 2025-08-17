#pragma once

#include "Zinet/VulkanRenderer/Tests/ZtTestUtils.hpp"

#include "Zinet/VulkanRenderer/ZtShaderModule.hpp"
#include "Zinet/VulkanRenderer/ZtShadersCompiler.hpp"
#include "Zinet/VulkanRenderer/ZtInstance.hpp"
#include "Zinet/VulkanRenderer/ZtDebugUtilsMessenger.hpp"
#include "Zinet/VulkanRenderer/ZtPhysicalDevice.hpp"
#include "Zinet/VulkanRenderer/ZtDevice.hpp"

#include <gtest/gtest.h>

#include <vulkan/vulkan.h>

#include "Zinet/Core/ZtPaths.hpp"

namespace zt::vulkan_renderer::tests
{
	class ShaderModuleTests : public ::testing::Test
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

			ASSERT_TRUE(device.create(instance, physicalDevice, Surface{ nullptr }));
		}

		void TearDown() override
		{
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
		ShaderModule shaderModule{ nullptr };

		static_assert(VulkanObjectDecoratorStaticTest<ShaderModule, VkShaderModule>());
	};

	TEST_F(ShaderModuleTests, PassTest)
	{
		ShadersCompiler shadersCompiler;

		const auto contentFolderPath = core::Paths::CurrentProjectRootPath() / "test_files";

		const auto vertShaderFilePath = contentFolderPath / "simple_triangle_shader.vert";
		const auto vertShaderResult = shadersCompiler.compileFromFile(vertShaderFilePath, ShaderType::Vertex);
		ASSERT_FALSE(vertShaderResult.empty());

		ASSERT_TRUE(shaderModule.create(device, vertShaderResult));
		ASSERT_TRUE(shaderModule.isValid());

		VkPipelineShaderStageCreateInfo shaderStageCreateInfo = shaderModule.createPipelineShaderStageCreateInfo(ShaderType::Vertex);
		EXPECT_EQ(shaderStageCreateInfo.sType, VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO);
		EXPECT_EQ(shaderStageCreateInfo.stage, VK_SHADER_STAGE_VERTEX_BIT);
		EXPECT_EQ(shaderStageCreateInfo.module, shaderModule.get());
		EXPECT_EQ(shaderStageCreateInfo.pName, std::string_view{ "main" });

		shaderModule.destroy(device);
	}
}