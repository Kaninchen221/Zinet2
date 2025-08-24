#include "Zinet/Gameplay/Editor/ZtEditorMetrics.hpp"

#include "Zinet/Gameplay/Editor/ZtEditorConfig.hpp"
#include "Zinet/Gameplay/Systems/ZtSystemRenderer.hpp"
#include "Zinet/Gameplay/ZtEngineContext.hpp"

#include "Zinet/Core/ZtImgui.hpp"

namespace zt::gameplay
{

	void EditorMetrics::show()
	{
		if (ImGui::Begin("Metrics", &shouldShow))
		{
			auto& io = ImGui::GetIO();
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

			gpuMetrics();
		}
		ImGui::End();
	}

	void EditorMetrics::gpuMetrics()
	{
		if (ImGui::CollapsingHeader("GPU Stats"))
		{
			gpuMemoryMetrics();
		}
	}

	void EditorMetrics::gpuMemoryMetrics()
	{
		ImGui::Indent(EditorConfig::IndentValue);
		if (ImGui::CollapsingHeader("Memory (Slow)"))
		{
			auto& engineContext = EngineContext::Get();
			auto systemRenderer = engineContext.getSystem<SystemRenderer>();
			if (!systemRenderer)
			{
				Logger->error("SystemRenderer not found");
				return;
			}

			auto& vma = systemRenderer->getRenderer().getRendererContext().vma;
			vma.updateBuildStatsString();
			ImGui::TextFMT("VMA Stats: \n{}", vma.getBuildStatsString());
		}
		ImGui::Unindent(EditorConfig::IndentValue);
	}

}