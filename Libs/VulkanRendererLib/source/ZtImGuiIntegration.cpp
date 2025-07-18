#include "Zinet/VulkanRenderer/ZtImGuiIntegration.hpp"
#include "Zinet/VulkanRenderer/ZtRendererContext.hpp"
#include "Zinet/VulkanRenderer/ZtDescriptorPool.hpp"
#include "Zinet/VulkanRenderer/ZtQueueUtils.hpp"
#include "Zinet/VulkanRenderer/ZtCommandBuffer.hpp"

#include "Zinet/Window/ZtWindow.hpp"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>

#include <array>

namespace zt::vulkan_renderer
{
	bool ImGuiIntegration::init(const RendererContext& rendererContext, wd::Window& window) ZINET_API_POST
	{
		{
			// Copied from imgui demo
			const std::array<VkDescriptorPoolSize, 11> poolSizes
			{
				VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
				{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
				{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
				{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
				{ VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
				{ VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
				{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
				{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
				{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
				{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
				{ VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
			};

			const VkDescriptorPoolCreateInfo createInfo{
				.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
				.pNext = nullptr,
				.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT,
				.maxSets = 1000,
				.poolSizeCount = static_cast<uint32_t>(poolSizes.size()),
				.pPoolSizes = poolSizes.data()
			};

			descriptorPool.create(rendererContext.device, createInfo);
		}

		ImGui::CreateContext();

		if (!ImGui_ImplGlfw_InitForVulkan(window.getInternal(), true))
		{
			Logger->error("ImGui_ImplGlfw_InitForVulkan returned false");
			return false;
		}
		
		ImGui_ImplVulkan_InitInfo init_info = {};
		init_info.Instance = rendererContext.instance.get();
		init_info.PhysicalDevice = rendererContext.physicalDevice.get();
		init_info.Device = rendererContext.device.get();
		init_info.Queue = rendererContext.queue.get();
		init_info.DescriptorPool = descriptorPool.get();
		init_info.MinImageCount = 3;
		init_info.ImageCount = 3;
		init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
		
		if (!ImGui_ImplVulkan_Init(&init_info, rendererContext.renderPass.get()))
		{
			Logger->error("ImGui_ImplVulkan_Init returned false");
			return false;
		}

		const auto commands = [](const CommandBuffer& commandBuffer)
		{
			ImGui_ImplVulkan_CreateFontsTexture(commandBuffer.get());
		};
		SubmitSingleCommandBufferWaitIdle(rendererContext, commands);
		
		ImGui_ImplVulkan_DestroyFontUploadObjects();

		return true;
	}

	void ImGuiIntegration::deinit(const RendererContext& rendererContext) ZINET_API_POST
	{
		descriptorPool.destroy(rendererContext.device);
		ImGui_ImplGlfw_Shutdown();
		ImGui_ImplVulkan_Shutdown();
	}

	void ImGuiIntegration::prepareRenderData() const ZINET_API_POST
	{
		ImGui::Render();
	}

	void ImGuiIntegration::ImplSpecificNewFrame() ZINET_API_POST
	{
		ImGui_ImplVulkan_NewFrame();
		ImGui_ImplGlfw_NewFrame();
	}

	void ImGuiIntegration::DrawCommand(const CommandBuffer& commandBuffer) ZINET_API_POST
	{
		auto drawData = ImGui::GetDrawData();
		if (drawData)
			ImGui_ImplVulkan_RenderDrawData(drawData, commandBuffer.get());
	}

}
