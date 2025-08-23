#include "Zinet/Gameplay/Systems/ZtSystemRenderer.hpp"
#include "Zinet/Gameplay/ZtEngineContext.hpp"

#include <imgui.h>

#include "Zinet/Core/ZtPaths.hpp"

namespace zt::gameplay
{
	bool SystemRenderer::init()
	{
		System::init();

		auto& engineContext = EngineContext::Get();

		renderer.getRendererContext().instance.setEnableValidationLayers(ZINET_DEBUG);
		if (!renderer.init(engineContext.getWindow()))
			return false;

		if (UseImGui)
		{
			if (!imGuiIntegration.init(renderer.getRendererContext(), engineContext.getWindow()))
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

		return true;
	}

	bool SystemRenderer::deinit()
	{
		if (!System::deinit())
			return false;

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

		return true;
	}

	void SystemRenderer::update()
	{
		System::update();

		if (nodes.size() == 0 && drawInfo.additionalCommands.empty())
			return;

		if (vertexShader && vertexShader.getAssetHandle()->isLoaded() &&
			fragmentShader && fragmentShader.getAssetHandle()->isLoaded())
		{
			drawInfo.vertexShaderModule = &vertexShader.getAssetHandle()->shaderModule;
			drawInfo.fragmentShaderModule = &fragmentShader.getAssetHandle()->shaderModule;

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
				drawInfo.drawCallDescriptorInfo += node2D->getDrawCallDescriptorInfos();
				drawInfo.instances++;
			}
		}

		// TODO: Add possibility to recreate graphics pipeline
		//if (renderer.getGraphicsPipeline().isValid())
		//{
		//	renderer.getRendererContext().device.waitIdle();
		//	renderer.getGraphicsPipeline().destroy(renderer.getRendererContext());
		//}

		renderer.createPipeline(drawInfo);
		if (!Ensure(renderer.getGraphicsPipeline().isValid()))
		{
			Logger->error("Graphics Pipeline is invalid");
			return;
		}

		renderer.beginFrame();

		imGuiIntegration.prepareRenderData();

		renderer.draw(drawInfo);

		renderer.submit();

		renderer.endFrame();
	}

	void SystemRenderer::show()
	{
		System::show();

		vertexShader.show();
		fragmentShader.show();
		if (camera)
			camera->show();
	}

}