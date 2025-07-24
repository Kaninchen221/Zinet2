#include "Zinet/Gameplay/Nodes/ZtNodeEditor.hpp"
#include "Zinet/Gameplay/ZtEngineContext.hpp"

#include <imgui.h>

namespace zt::gameplay
{
	using Path = std::filesystem::path;

	void AbstractObject::imGui()  ZINET_API_POST
	{
		ImGui::Text("Inspect content");
	}

	void EditorBrowserInspector::show() ZINET_API_POST
	{
		const ImVec2 size = {};
		if (ImGui::BeginChild("Inspector", size, true))
		{
			ImGui::Text("Inspector of:");

		}
		ImGui::EndChild();
	}

	void EditorBrowserList::show() ZINET_API_POST
	{
		const ImVec2 size = {};
		if (ImGui::BeginChild("List", size, true))
		{
			ImGui::Text("List of: ");

		}
		ImGui::EndChild();
	}

	void EditorBrowser::show() ZINET_API_POST
	{
		const int columns = 2;
		ImGuiTableFlags flags = ImGuiTableFlags_Resizable;
		if (ImGui::BeginTable("EditorBrowser", columns, flags))
		{
			ImGui::TableSetupColumn("List", ImGuiTableColumnFlags_WidthFixed);
			ImGui::TableSetupColumn("Inspector", ImGuiTableFlags_SizingStretchProp);

			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			list.show();
			ImGui::TableNextColumn();
			inspector.show();

			ImGui::EndTable();
		}
	}

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
				if (ImGui::BeginChild("assets panel", ImVec2(ImGui::GetContentRegionAvail().x / EditorConfig::ListMenuWidthDiv, 0), true))
				{
					ImGui::Text("Assets");
					ImGui::Separator();
					int index = 0;
					for (auto& [key, asset] : engineContext.assetsStorage.getAssets())
					{
						if (!searchSubString.empty() && !key.contains(searchSubString) && selectedAssetKey != key)
							continue;

						auto assetName = fmt::format("{}", asset->metaData.value("fileName", "Couldn't read 'fileName'"));
						if (ImGui::Selectable(key.c_str(), selectedAssetKey == key))
						{
							selectedAssetIndex = index;
							selectedAssetKey = key;
							break;
						}

						if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
						{
							void* payloadData = asset.get();
							ImGui::SetDragDropPayload(AssetPayloadType, &payloadData, sizeof(void*));
							ImGui::Text(fmt::format("Asset: {}", assetName).c_str());
							ImGui::EndDragDropSource();
						}

						++index;
					}
				}
				ImGui::EndChild();
			}
			ImGui::SameLine();

			// Selected asset data
			{
				ImGui::BeginGroup();
				if (ImGui::BeginChild("item view", ImVec2(0, 0)))
				{
					ImGui::Text("Asset Data");
					ImGui::Separator();

					auto& assets = engineContext.assetsStorage.getAssets();
					auto assetIt = assets.find(selectedAssetKey);
					if (assetIt != assets.end())
					{
						if (assetIt->second)
						{
							assetIt->second->imGuiAssetInspect();
						}
						else
						{
							Logger->warn("Invalid asset in the EditorAssetsList");
						}
					}
				}
				ImGui::EndChild();
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
					const auto& name = system->getName();
					if (!searchSubString.empty() && !name.contains(searchSubString))
						continue;

					if (ImGui::Selectable(system->getName().c_str(), selectedSystemIndex == systemIndex))
					{
						Logger->info("Selected system: {}", system->getName());
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

		if (assetsList.shouldShow)
			assetsList.show();

		if (metrics.shouldShow)
			metrics.show();

		if (nodesList.shouldShow)
			nodesList.show();

		if (systemsList.shouldShow)
			systemsList.show();

		editorBrowserTest.show();

		ImGui::ShowDemoWindow();
	}

	void NodeEditor::showToolsMenu() ZINET_API_POST
	{
		if (ImGui::MenuItem("Assets List", nullptr, &assetsList.shouldShow)) {}
		if (ImGui::MenuItem("Metrics", nullptr, &metrics.shouldShow)) {}
		if (ImGui::MenuItem("Nodes List", nullptr, &nodesList.shouldShow)) {}
		if (ImGui::MenuItem("Systems List", nullptr, &systemsList.shouldShow)) {}
	}

}