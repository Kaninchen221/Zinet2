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

	bool SystemRenderer::init() ZINET_API_POST
	{
		System::init();

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

	void SystemRenderer::deinit() ZINET_API_POST
	{
		System::deinit();

		const auto& device = renderer.getRendererContext().device;
		device.waitIdle();

		vertexShaderModule.destroy(renderer.getRendererContext().device);
		fragmentShaderModule.destroy(renderer.getRendererContext().device);

		vertexBuffer.destroy(renderer.getRendererContext().vma);
		indexBuffer.destroy(renderer.getRendererContext().vma);

		imGuiIntegration.deinit(renderer.getRendererContext());
		renderer.deinit();
	}

	void SystemRenderer::update() ZINET_API_POST
	{
		System::update();

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

	void SystemRenderer::imGui() ZINET_API_POST
	{
		System::imGui();

		vertexShader.show();
		fragmentShader.show();
	}

}