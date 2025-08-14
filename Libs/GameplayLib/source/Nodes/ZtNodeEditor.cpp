#include "Zinet/Gameplay/Nodes/ZtNodeEditor.hpp"
#include "Zinet/Gameplay/ZtEngineContext.hpp"

#include "Zinet/Gameplay/Editor/ZtEditorConfig.hpp"

#include <imgui.h>

#include "Zinet/Core/ZtPaths.hpp"

namespace zt::gameplay
{
	void EditorMetrics::show()
	{
		if (ImGui::Begin("Metrics", &shouldShow))
		{
			auto& io = ImGui::GetIO();
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
		}
		ImGui::End();
	}

	void NodeEditor::imGui()
	{
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("Tools"))
			{
				showToolsMenu();
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}

		if (assetsList.isOpen)
		{
			auto& engineContext = EngineContext::Get();
			auto assets = engineContext.getAssetsStorage().getAssets();
			assetsList.show(assets, CreateObjectBrowserListElement);
		}

		if (metrics.shouldShow)
			metrics.show();

		if (nodesList.isOpen)
		{
			auto& engineContext = EngineContext::Get();
			auto rootNode = engineContext.getRootNode();
			std::vector<ObjectHandle<Node>> nodes{ rootNode };
			nodesList.show(nodes, CreateNodeBrowserListElement);
		}

		if (systemsList.isOpen)
		{
			auto& engineContext = EngineContext::Get();
			systemsList.show(engineContext.getSystems(), CreateObjectBrowserListElement);
		}

		ImGui::ShowDemoWindow();
	}

	void NodeEditor::showToolsMenu()
	{
		if (ImGui::MenuItem("Assets List", nullptr, &assetsList.isOpen)) {}
		if (ImGui::MenuItem("Metrics", nullptr, &metrics.shouldShow)) {}
		if (ImGui::MenuItem("Nodes List", nullptr, &nodesList.isOpen)) {}
		if (ImGui::MenuItem("Systems List", nullptr, &systemsList.isOpen)) {}
	}

}