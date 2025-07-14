#include "Zinet/Gameplay/Systems/ZtSystemRenderer.hpp"
#include "Zinet/Gameplay/ZtEngineContext.hpp"

#include <imgui.h>

#include "Zinet/Core/ZtPaths.hpp"

namespace zt::gameplay
{
	// TODO: Remove that after using assets
	vr::ShaderModule SystemRenderer::createShaderModule(std::string_view sourceCodeFileName, vr::ShaderType shaderType)
	{
		vr::ShadersCompiler shadersCompiler;
		vr::ShaderModule shaderModule{ nullptr };

		const auto contentFolderPath = core::Paths::CurrentProjectRootPath() / "test_files";

		const auto fullPath = contentFolderPath / sourceCodeFileName;
		const auto compileResult = shadersCompiler.compileFromFile(fullPath, shaderType);
		if (compileResult.GetCompilationStatus() != shaderc_compilation_status_success)
			return shaderModule;

		const auto& device = renderer.getRendererContext().device;
		shaderModule.create(device, compileResult);

		return shaderModule;
	}

	bool SystemRenderer::init() noexcept
	{
		auto& engineContext = EngineContext::Get();

		if (!renderer.init(engineContext.window))
			return false;

		if (!imGuiIntegration.init(renderer.getRendererContext(), engineContext.window))
			return false;

		drawInfo.additionalCommands = { vr::ImGuiIntegration::DrawCommand };

		// TODO: Remove that after using assets
		vertexShaderModule = createShaderModule("shader.vert", vr::ShaderType::Vertex);
		fragmentShaderModule = createShaderModule("shader.frag", vr::ShaderType::Fragment);

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

	void SystemRenderer::deinit() noexcept
	{
		const auto& device = renderer.getRendererContext().device;
		device.waitIdle();

		vertexShaderModule.destroy(renderer.getRendererContext().device);
		fragmentShaderModule.destroy(renderer.getRendererContext().device);

		vertexBuffer.destroy(renderer.getRendererContext().vma);
		indexBuffer.destroy(renderer.getRendererContext().vma);

		imGuiIntegration.deinit(renderer.getRendererContext());
		renderer.deinit();
	}

	void SystemRenderer::addNode(const NodeWeakHandle<NodeT>& node) noexcept
	{
		ClassBaseT::addNode(node);

		drawInfo.instances = static_cast<uint32_t>(nodes.size());
	}

	void SystemRenderer::update() noexcept
	{
		if (drawInfo.instances == 0 && drawInfo.additionalCommands.empty())
			return;

		renderer.createPipeline(drawInfo);
		renderer.getGraphicsPipeline().isValid();

		renderer.beginFrame();

		imGuiIntegration.prepareRenderData();

		renderer.draw(drawInfo);
		renderer.getGraphicsPipeline().commandBuffer;

		renderer.submit();

		renderer.endFrame();
	}

}