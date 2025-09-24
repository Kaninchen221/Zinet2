#include "Zinet/Gameplay/Systems/ZtSystemRenderer.hpp"
#include "Zinet/Gameplay/Systems/ZtSystemWindow.hpp"
#include "Zinet/Gameplay/ZtEngineContext.hpp"

#include <imgui.h>

#include "Zinet/Core/ZtPaths.hpp"
#include "Zinet/Core/ZtTimeLog.hpp"

namespace zt::gameplay
{
	using namespace zt::vulkan_renderer;

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
			if (!imGuiIntegration.init(rendererContext, window))
				return false;

			drawInfo.additionalCommands = { ImGuiIntegration::DrawCommand };
		}

		const DrawInfo::Vertices vertices = {
			{{-0.5f, 0.5f, 1.f}, {1.0f, 1.0f, 1.0f, 1.f}, {0.f, 0.f}},
			{{0.5f,  0.5f, 1.f}, {1.0f, 1.0f, 1.0f, 1.f}, {1.f, 0.f}},
			{{0.5f,  -0.5f,  1.f}, {1.0f, 1.0f, 1.0f, 1.f}, {1.f, 1.f}},
			{{-0.5f, -0.5f,  1.f}, {1.0f, 1.0f, 1.0f, 1.f}, {0.f, 1.f}}
		};

		const auto vertexBufferCreateInfo = Buffer::GetVertexBufferCreateInfo(vertices);
		vertexBuffer.create(vma, vertexBufferCreateInfo);
		vertexBuffer.fillWithSTDContainer(vma, vertices);

		const DrawInfo::Indices indices =
		{
			0, 1, 2,
			2, 3, 0
		};

		const auto indexBufferCreateInfo = Buffer::GetIndexBufferCreateInfo(indices);
		indexBuffer.create(vma, indexBufferCreateInfo);
		indexBuffer.fillWithSTDContainer(vma, indices);

		graphicsPipelineCreateInfo.descriptorSetsCount = rendererContext.getDisplayImagesCount();

		initialized = true;
		return true;
	}

	bool SystemRenderer::deinit()
	{
		if (!System::deinit())
			return false;

		auto& rendererContext = renderer.getRendererContext();
		auto& device = rendererContext.getDevice();
		auto& vma = rendererContext.getVMA();

		device.waitIdle();

		vertexBuffer.destroy(vma);
		indexBuffer.destroy(vma);

		if (UseImGui)
		{
			imGuiIntegration.deinit(rendererContext);
		}

		graphicsPipeline.destroy(rendererContext);

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

		// Add descriptor info to the object descriptor
		graphicsPipelineCreateInfo.descriptorInfos[1] += node2D->getDescriptorInfo();
		drawInfo.instances += node2D->getInstancesCount();
	}

	void SystemRenderer::update()
	{
		System::update();

		if (renderer.shouldBePaused())
			return;

		if (nodes.size() == 0 && drawInfo.additionalCommands.empty())
			return;

		if (!graphicsPipeline.isValid())
		{
			if (vertexShader)
				graphicsPipelineCreateInfo.shaderModules[ShaderType::Vertex] = vertexShader->getShaderModule();

			if (fragmentShader)
				graphicsPipelineCreateInfo.shaderModules[ShaderType::Fragment] = fragmentShader->getShaderModule();

			graphicsPipeline.create(graphicsPipelineCreateInfo);
		}

		ZT_TIME_LOG(
			renderer.nextImage();
		);

		if (GetUseImGui())
			imGuiIntegration.prepareRenderData();

		ZT_TIME_LOG(
			renderer.draw(graphicsPipeline, drawInfo);
		);

		ZT_TIME_LOG(
			renderer.submitCurrentDisplayImage();
		);

		ZT_TIME_LOG(
			renderer.displayCurrentImage();
		);
	}

	void SystemRenderer::show()
	{
		System::show();

		vertexShader.show();
		fragmentShader.show();
		if (camera)
			camera->show();
	}

	void SystemRenderer::setCameraNode(ObjectHandle<NodeCamera> newCamera) noexcept
	{
		if (newCamera)
		{
			camera = newCamera;
			graphicsPipelineCreateInfo.descriptorInfos[0] = camera->getDescriptorInfo();
		}
		else
		{
			Logger->error("You passed an invalid node camera to the system renderer");
		}
	}

}