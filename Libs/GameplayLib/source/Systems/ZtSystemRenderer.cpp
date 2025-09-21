#include "Zinet/Gameplay/Systems/ZtSystemRenderer.hpp"
#include "Zinet/Gameplay/Systems/ZtSystemWindow.hpp"
#include "Zinet/Gameplay/ZtEngineContext.hpp"

#include <imgui.h>

#include "Zinet/Core/ZtPaths.hpp"

namespace zt::gameplay
{
	bool SystemRenderer::init()
	{
		auto& engineContext = EngineContext::Get();
		auto& rendererContext = renderer.getRendererContext();
		auto& instance = rendererContext.getInstance();
		auto& vma = rendererContext.getVMA();

		auto systemWindow = engineContext.getSystem<SystemWindow>();
		if (!systemWindow)
		{
			Logger->error("SystemWindow not found");
			return Ensure(false);
		}

		if (!systemWindow->isInitialized())
		{
			Logger->error("SystemWindow must be initialized before system renderer");
			return Ensure(false);
		}

		auto& window = systemWindow->getWindow();
		if (!window.isOpen())
		{
			Logger->error("Window is not open");
			return Ensure(false);
		}

		instance.setEnableValidationLayers(ZINET_DEBUG);
		if (!renderer.init(window))
			return false;

		if (UseImGui)
		{
			if (!imGuiIntegration.init(renderer.getRendererContext(), window))
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
		vertexBuffer.createBuffer(vma, vertexBufferCreateInfo);
		vertexBuffer.fillWithSTDContainer(vma, vertices);

		const vr::DrawInfo::Indices indices =
		{
			0, 1, 2,
			2, 3, 0
		};

		const auto indexBufferCreateInfo = vr::Buffer::GetIndexBufferCreateInfo(indices);
		indexBuffer.createBuffer(vma, indexBufferCreateInfo);
		indexBuffer.fillWithSTDContainer(vma, indices);

		initialized = true;
		return true;
	}

	bool SystemRenderer::deinit()
	{
		if (!System::deinit())
			return false;

		auto& device = renderer.getRendererContext().getDevice();
		auto& vma = renderer.getRendererContext().getVMA();

		device.waitIdle();

		vertexShaderModule.destroy(device);
		fragmentShaderModule.destroy(device);

		vertexBuffer.destroy(vma);
		indexBuffer.destroy(vma);

		if (UseImGui)
		{
			imGuiIntegration.deinit(renderer.getRendererContext());
		}

		renderer.deinit();

		return true;
	}

	void SystemRenderer::addNode(const ObjectWeakHandle<Node>& node)
	{
		if (!node)
			return;

		System::addNode(node);

		auto node2D = dynamic_cast<Node2D*>(node.get());
		if (!node2D)
			return;

		drawInfo.objectDescriptorInfo += node2D->getDescriptorInfo();
		drawInfo.instances++;
	}

	void SystemRenderer::update()
	{
		System::update();

		if (renderer.shouldBePaused())
			return;

		if (nodes.size() == 0 && drawInfo.additionalCommands.empty())
			return;

		if (vertexShader && vertexShader.getAssetHandle()->isLoaded() &&
			fragmentShader && fragmentShader.getAssetHandle()->isLoaded())
		{
			drawInfo.vertexShaderModule = &vertexShader.getAssetHandle()->shaderModule;
			drawInfo.fragmentShaderModule = &fragmentShader.getAssetHandle()->shaderModule;
		}
		else
		{
			return;
		}

		static bool addedCamera = false;
		if (camera && !addedCamera)
		{
			addedCamera = true;
			drawInfo.pipelineDescriptorInfo += camera->getDescriptorInfo();
		}

		renderer.createPipeline(drawInfo);
		if (!Ensure(renderer.getGraphicsPipeline().isValid()))
		{
			Logger->error("Graphics Pipeline is invalid");
			return;
		}

		renderer.nextImage();

		if (GetUseImGui())
			imGuiIntegration.prepareRenderData();

		renderer.draw(drawInfo);

		renderer.submitDrawInfo();

		renderer.displayCurrentImage();
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