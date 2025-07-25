#include "Zinet/Gameplay/Nodes/ZtNodeEditor.hpp"
#include "Zinet/Gameplay/ZtEngineContext.hpp"

#include <imgui.h>

#include "Zinet/Core/ZtPaths.hpp"

namespace zt::gameplay
{
	void EditorMetrics::show() ZINET_API_POST
	{
		if (ImGui::Begin("Metrics", &shouldShow))
		{
			auto& io = ImGui::GetIO();
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
		}
		ImGui::End();
	}

	void EditorNodesList::show() ZINET_API_POST
	{
		if (ImGui::Begin("Nodes List", &shouldShow))
		{
			searchSubString = textSearchBar.show();
			ImGui::Separator();

			auto& engineContext = EngineContext::Get();
			auto& rootNode = engineContext.rootNode;

			if (ImGui::BeginChild("Nodes list", ImVec2(ImGui::GetContentRegionAvail().x / EditorConfig::ListMenuWidthDiv, 0), true))
			{
				const int deep = 1;
				generateSelectable(rootNode, deep);
			}
			ImGui::EndChild();

			ImGui::SameLine();

			if (ImGui::BeginChild("Node Inspector", {}, false))
			{
				ImGui::Text("Selected Node");
				ImGui::Separator();
				if (selectedNode)
				{
					selectedNode->imGuiNodeInspect();
				}
			}
			ImGui::EndChild();
		}
		ImGui::End();
	}

	void EditorNodesList::generateSelectable(NodeHandle<>& node, int deep) ZINET_API_POST
	{
		if (!node)
			return;

		const bool showNode = searchSubString.empty() || node->getName().contains(searchSubString);
		if (showNode)
		{
			if (ImGui::Selectable(node->getName().c_str(), selectedNode == node))
			{
				Logger->info("Selected node: {}", node->getName());
				selectedNode = node;
			}
		}

		ImGui::Indent(static_cast<float>(deep) * EditorConfig::IndentMulti);

		for (auto& childNode : node->getChildren())
		{
			generateSelectable(childNode, deep + 1);
		}

		ImGui::Unindent(static_cast<float>(deep) * EditorConfig::IndentMulti);
	}

	void EditorSystemsList::show() ZINET_API_POST
	{
		if (ImGui::Begin("Systems List", &shouldShow))
		{
			searchSubString = textSearchBar.show();
			ImGui::Separator();

			auto& engineContext = EngineContext::Get();

			if (ImGui::BeginChild("Systems", ImVec2(ImGui::GetContentRegionAvail().x / EditorConfig::ListMenuWidthDiv, 0), true))
			{
				int32_t systemIndex = 0;
				for (auto& system : engineContext.getSystems())
				{
					const auto& name = system->getDisplayName();
					if (!searchSubString.empty() && !name.contains(searchSubString))
						continue;

					if (ImGui::Selectable(system->getDisplayName().c_str(), selectedSystemIndex == systemIndex))
					{
						Logger->info("Selected system: {}", system->getDisplayName());
						selectedSystemIndex = systemIndex;
					}

					++systemIndex;
				}
			}
			ImGui::EndChild();

			ImGui::SameLine();

			if (ImGui::BeginChild("System Inspector", {}, false))
			{
				ImGui::Text("Selected System");
				ImGui::Separator();
				if (selectedSystemIndex != invalidSystemIndex)
				{
					auto& system = engineContext.getSystems()[selectedSystemIndex];
					system->imGui();
				}
			}
			ImGui::EndChild();
		}
		ImGui::End();
	}

	void NodeEditor::imGui() ZINET_API_POST
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
			auto assets = engineContext.assetsStorage.getAssets();
			assetsList.show(assets);
		}

		if (metrics.shouldShow)
			metrics.show();

		if (nodesList.shouldShow)
			nodesList.show();

		if (systemsList.isOpen)
		{
			auto& engineContext = EngineContext::Get();
			systemsList.show(engineContext.getSystems());
		}

		ImGui::ShowDemoWindow();
	}

	void NodeEditor::showToolsMenu() ZINET_API_POST
	{
		if (ImGui::MenuItem("Assets List", nullptr, &assetsList.isOpen)) {}
		if (ImGui::MenuItem("Metrics", nullptr, &metrics.shouldShow)) {}
		if (ImGui::MenuItem("Nodes List", nullptr, &nodesList.shouldShow)) {}
		if (ImGui::MenuItem("Systems List", nullptr, &systemsList.isOpen)) {}
	}

}