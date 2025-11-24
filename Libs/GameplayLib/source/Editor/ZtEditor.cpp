#include "Zinet/Gameplay/Editor/ZtEditor.hpp"

#include <imgui.h>

#include "Zinet/Window/ZtWindow.hpp"

#include "Zinet/Core/ZtImgui.hpp"
#include "Zinet/Core/ZtUtils.hpp"

using namespace zt::core;

namespace zt::gameplay
{
	void Editor::EntryPoint(ecs::ConstResource<wd::Window> windowRes,
		ecs::ConstResource<ecs::Graph> graphRes)
	{
		if (!windowRes || !windowRes->isOpen())
			return;

#	if ZINET_DEBUG
		ImGui::ShowDemoWindow();
#	endif

		MainMenuBar();

		if (OpenScheduleGraphWindow)
		{
			ScheduleGraphWindow(graphRes);
		}
	}

	void Editor::MainMenuBar()
	{
		ImGui::BeginMainMenuBar();

		ToolsSubMenu();

		ImGui::EndMainMenuBar();
	}

	void Editor::ToolsSubMenu()
	{
		if (!ImGui::BeginMenu("Tools"))
			return;

		ImGui::MenuItem("Schedule Graph", nullptr, &OpenScheduleGraphWindow, true);

		ImGui::EndMenu();
	}

	void Editor::ScheduleGraphWindow(ecs::ConstResource<core::ecs::Graph> graphRes)
	{
		const ImGuiWindowFlags windowFlags = ImGuiWindowFlags_HorizontalScrollbar;
		if (ImGui::Begin("Schedule Graph", &OpenScheduleGraphWindow, windowFlags))
		{
			auto& layers = graphRes->layers;
			ImGui::TextFMT("Layers count: {}", layers.size());

			const ImGuiChildFlags childFlags = ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY;
			if (ImGui::BeginChild("Schedule Graph Table Child", ImVec2(0, 0), childFlags))
			{
				const auto columnsCount = static_cast<int32_t>(layers.size());

				const ImGuiTableFlags tableFlags = 
					ImGuiTableFlags_Borders |
					ImGuiTableFlags_SizingFixedFit;

				if (ImGui::BeginTable("ScheduleGraphTable", columnsCount, tableFlags))
				{
					ImGui::TableNextRow();

					for (size_t layerIndex = 0; auto& layer : layers)
					{
						ImGui::TableNextColumn();

						ImGui::TextFMT("Layer {}", layerIndex);
						ImGui::TextFMT("Nodes count: {}", layer.nodes.size());

						ImGui::Separator();
						for (auto& node : layer.nodes)
						{
							ImGui::TextFMT("{}", node.typeInfo->name());
							// TODO: Add info how long it takes to execute each node
						}

						++layerIndex;
					}

					ImGui::EndTable();
				}
			}

			ImGui::EndChild();
		}

		ImGui::End();
	}

}