#include "Zinet/Gameplay/Editor/ZtEditorMetrics.hpp"

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

			if (io.Framerate > imGuiFPSGraph.getMaxValue())
				imGuiFPSGraph.setMaxValue(io.Framerate);

			imGuiFPSGraph.update(io.Framerate);
			imGuiFPSGraph.show();
		}
	}

	void EditorMetrics::cpuMetrics()
	{
		if (ImGui::CollapsingHeader("CPU Stats"))
		{
			// TODO: Impl
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
		// TODO: Impl
	}

}