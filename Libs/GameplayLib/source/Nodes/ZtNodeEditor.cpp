#include "Zinet/Gameplay/Nodes/ZtNodeEditor.hpp"
#include "Zinet/Gameplay/ZtEngineContext.hpp"

#include <imgui.h>

namespace zt::gameplay
{
	using Path = std::filesystem::path;

	std::string_view EditorTextSearchBar::show() ZINET_API_POST
	{
		ImGui::InputText("SearchBar", assetsListBuffer.data(), assetsListBuffer.size());
		return std::string_view{ assetsListBuffer.begin(), std::ranges::find(assetsListBuffer, '\0') };
	}

	void EditorAssetsList::show() ZINET_API_POST
	{
		using Path = std::filesystem::path;

		if (ImGui::Begin("Assets List", &shouldShow))
		{
			const auto searchSubString = textSearchBar.show();
			auto& engineContext = EngineContext::Get();

			// List of assets
			{
				if (ImGui::BeginChild("assets panel", ImVec2(150, 0), true))
				{
					ImGui::Text("Assets");
					ImGui::Separator();
					int index = 0;
					for (auto& [key, asset] : engineContext.assetsStorage.getAssets())
					{
						if (!searchSubString.empty() && !key.contains(searchSubString) && selectedAssetKey != key)
							continue;

						auto assetName = fmt::format("{}", asset->metaData.value("fileName", "Couldn't read 'fileName'"));
						if (ImGui::Selectable(assetName.c_str(), selectedAssetKey == key))
						{
							selectedAssetIndex = index;
							selectedAssetKey = key;
							break;
						}

						++index;
					}
					ImGui::EndChild();
				}
			}
			ImGui::SameLine();

			// Selected asset data
			{
				ImGui::BeginGroup();
				if (ImGui::BeginChild("item view", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()))) // Leave room for 1 line below us
				{
					ImGui::Text("Asset Data");
					ImGui::Separator();

					auto& assets = engineContext.assetsStorage.getAssets();
					auto assetIt = assets.find(selectedAssetKey);
					if (assetIt != assets.end())
					{

						if (ImGui::BeginTable("table1", 2, {}/*flags*/))
						{
							for (auto& [key, value] : assetIt->second->metaData.items())
							{
								ImGui::TableNextRow();
								ImGui::TableSetColumnIndex(0);
								ImGui::TextUnformatted(key.c_str());
								ImGui::TableSetColumnIndex(1);
								auto valueText = value.dump();
								ImGui::TextUnformatted(valueText.c_str());
							}
							ImGui::EndTable();
						}
					}

					ImGui::EndChild();
				}
				ImGui::EndGroup();
			}
		}
		ImGui::End();
	}

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

			const int deep = 1;
			generateSelectable(rootNode, deep);
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

		ImGui::Indent(static_cast<float>(deep) * 4.f);

		for (auto& childNode : node->getChildren())
		{
			generateSelectable(childNode, deep + 1);
		}

		ImGui::Unindent(static_cast<float>(deep) * 4.f);
	}

	void EditorNodeInspector::show(NodeHandle<> node) ZINET_API_POST
	{
		if (ImGui::Begin("Node Inspector"))
		{
			if (!node)
			{
				ImGui::End();
				return;
			}

			ImGui::Text(node->getName().c_str());
			ImGui::Separator();
			node->imGuiNodeInspect();
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

		if (assetsList.shouldShow)
			assetsList.show();

		if (metrics.shouldShow)
			metrics.show();

		if (nodesList.shouldShow)
			nodesList.show();
		else
			nodesList.selectedNode = {};

		if (nodesList.selectedNode)
			nodeInspector.show(nodesList.selectedNode);

		ImGui::ShowDemoWindow();
	}

	void NodeEditor::showToolsMenu() ZINET_API_POST
	{
		if (ImGui::MenuItem("Assets List", nullptr, &assetsList.shouldShow)) {}
		if (ImGui::MenuItem("Metrics", nullptr, &metrics.shouldShow)) {}
		if (ImGui::MenuItem("Nodes List", nullptr, &nodesList.shouldShow)) {}
	}

}