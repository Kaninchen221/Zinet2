#pragma once

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
			ASSERT_TRUE(physicalDevice.isValid());

			ASSERT_TRUE(device.create(physicalDevice, Surface{ nullptr }));
		}

		void TearDown() override
		{
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
		ShaderModule shaderModule{ nullptr };

		static_assert(std::is_base_of_v<VulkanObject<VkShaderModule>, ShaderModule>);

		static_assert(std::is_constructible_v<ShaderModule, VkShaderModule>);
		static_assert(!std::is_default_constructible_v<ShaderModule>);
		static_assert(!std::is_copy_constructible_v<ShaderModule>);
		static_assert(!std::is_copy_assignable_v<ShaderModule>);
		static_assert(std::is_move_constructible_v<ShaderModule>);
		static_assert(std::is_move_assignable_v<ShaderModule>);
		static_assert(std::is_destructible_v<ShaderModule>);
	};

	TEST_F(ShaderModuleTests, PassTest)
	{
		ShadersCompiler shadersCompiler;

		const auto contentFolderPath = core::Paths::CurrentProjectRootPath() / "test_files";

		const auto vertShaderFilePath = contentFolderPath / "simple_triangle_shader.vert";
		const auto vertShaderResult = shadersCompiler.compileFromFile(vertShaderFilePath, ShaderType::Vertex);
		ASSERT_EQ(vertShaderResult.GetCompilationStatus(), shaderc_compilation_status_success);

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