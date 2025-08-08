#include "Zinet/Gameplay/Systems/ZtSystemRenderer.hpp"
#include "Zinet/Gameplay/ZtEngineContext.hpp"

#include <imgui.h>

#include "Zinet/Core/ZtPaths.hpp"

namespace zt::gameplay
{
	bool SystemRenderer::init() ZINET_API_POST
	{
		System::init();

		auto& engineContext = EngineContext::Get();

		renderer.getRendererContext().instance.setEnableValidationLayers(false);
		if (!renderer.init(engineContext.window))
			return false;

		if (UseImGui)
		{
			if (!imGuiIntegration.init(renderer.getRendererContext(), engineContext.window))
				return false;

			drawInfo.additionalCommands = { vr::ImGuiIntegration::DrawCommand };
		}

		const vr::DrawInfo::Vertices vertices = {
			{{-0.5f, 0.5f, 1.f}, {1.0f, 1.0f, 1.0f, 1.f}, {0.f, 0.f}},
			{{0.5f,  0.5f, 1.f}, {1.0f, 1.0f, 1.0f, 1.f}, {1.f, 0.f}},
			{{0.5f,  -0.5f,  1.f}, {1.0f, 1.0f, 1.0f, 1.f}, {1.f, 1.f}},
			{{-0.5f, -0.5f,  1.f}, {1.0f, 1.0f, 1.0f, 1.f}, {0.f, 1.f}}
		};

		const auto vertexBufferCreateInfo = vr::Buffer::GetVertexBufferCreateInfo(vertices);
		vertexBuffer.createBuffer(vertexBufferCreateInfo, renderer.getRendererContext().vma);
		vertexBuffer.fillWithSTDContainer(vertices, renderer.getRendererContext().vma);

		const vr::DrawInfo::Indices indices =
		{
			0, 1, 2,
			2, 3, 0
		};

		const auto indexBufferCreateInfo = vr::Buffer::GetIndexBufferCreateInfo(indices);
		indexBuffer.createBuffer(indexBufferCreateInfo, renderer.getRendererContext().vma);
		indexBuffer.fillWithSTDContainer(indices, renderer.getRendererContext().vma);
		//

		return true;
	}

	void SystemRenderer::deinit() ZINET_API_POST
	{
		System::deinit();

		const auto& device = renderer.getRendererContext().device;
		device.waitIdle();

		vertexShaderModule.destroy(renderer.getRendererContext().device);
		fragmentShaderModule.destroy(renderer.getRendererContext().device);

		vertexBuffer.destroy(renderer.getRendererContext().vma);
		indexBuffer.destroy(renderer.getRendererContext().vma);

		if (UseImGui)
		{
			imGuiIntegration.deinit(renderer.getRendererContext());
		}

		renderer.deinit();
	}

	void SystemRenderer::update() ZINET_API_POST
	{
		System::update();

		if (nodes.size() == 0 && drawInfo.additionalCommands.empty())
			return;

		if (vertexShader && vertexShader.assetHandle->isLoaded() &&
			fragmentShader && fragmentShader.assetHandle->isLoaded())
		{
			drawInfo.vertexShaderModule = &vertexShader.assetHandle->shaderModule;
			drawInfo.fragmentShaderModule = &fragmentShader.assetHandle->shaderModule;

			drawInfo.pipelineDescriptorInfo = {};
			drawInfo.drawCallDescriptorInfo = {};
			drawInfo.instances = 0;
			for (const auto& node : nodes)
			{
				if (!node)
					continue;

				auto node2D = dynamic_cast<Node2D*>(node.get());
				if (!node2D)
					continue;

				drawInfo.pipelineDescriptorInfo += node2D->getPipelineDescriptorInfos();
				drawInfo.drawCallDescriptorInfo += node2D->getPipelineDescriptorInfos();
				drawInfo.instances++;
			}
		}

		renderer.createPipeline(drawInfo);
		renderer.getGraphicsPipeline().isValid();

		renderer.beginFrame();

		imGuiIntegration.prepareRenderData();

		renderer.draw(drawInfo);
		renderer.getGraphicsPipeline().commandBuffer;

		renderer.submit();

		renderer.endFrame();
	}

	void SystemRenderer::imGui() ZINET_API_POST
	{
		System::imGui();

		vertexShader.show();
		fragmentShader.show();
		if (camera)
			camera->imGui();
	}

}