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
			imGuiMetrics();
			cpuMetrics();
			gpuMetrics();
		}
		ImGui::End();
	}

	void EditorMetrics::imGuiMetrics()
	{
		if (ImGui::CollapsingHeader("ImGui Stats"))
		{
			auto& io = ImGui::GetIO();
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

			if (io.Framerate > imGuiFPSMax)
				imGuiFPSMax = io.Framerate;

			imGuiFPSPoints.push_back(io.Framerate);

			if (imGuiFPSPoints.size() > imGuiFPSMaxPoints)
				imGuiFPSPoints.erase(imGuiFPSPoints.begin());

			const int valuesOffset = 0;
			const ImVec2 size = ImVec2(ImGui::GetContentRegionAvail().x, 80.0f);
			ImGui::PlotLines("##0", imGuiFPSPoints.data(), static_cast<int>(imGuiFPSPoints.size()), valuesOffset, fmt::format("Points: {}", imGuiFPSPoints.size()).c_str(), imGuiFPSMin, imGuiFPSMax, size);
		}
	}

	void EditorMetrics::cpuMetrics()
	{
		if (ImGui::CollapsingHeader("CPU Stats"))
		{

		}
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