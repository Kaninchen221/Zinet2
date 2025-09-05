#include "Zinet/Gameplay/Nodes/ZtNodeEditor.hpp"
#include "Zinet/Gameplay/Systems/ZtSystemWindow.hpp"
#include "Zinet/Gameplay/ZtEngineContext.hpp"

#include "Zinet/Gameplay/Editor/ZtEditorConfig.hpp"

#include <imgui.h>

#include "Zinet/Core/ZtPaths.hpp"
#include "Zinet/Core/Assets/ZtAsset.hpp"

namespace zt::gameplay
{
	void NodeEditor::show()
	{
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("Tools"))
			{
				showToolsMenu();
				ImGui::EndMenu();
			}

			if (ImGui::MenuItem("Exit", nullptr, nullptr))
			{
				auto& engineContext = EngineContext::Get();
				auto windowSystem = engineContext.getSystem<SystemWindow>();
				windowSystem->getWindow().requestCloseWindow();
				engineContext.stopLooping();
			}

			ImGui::EndMainMenuBar();
		}

		if (assetsList.isOpen)
		{
			auto& engineContext = EngineContext::Get();
			auto assets = engineContext.getAssetsStorage().getAssets();
			assetsList.show(assets, CreateObjectBrowserListElement<core::AssetHandle<core::Asset>>);
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
			systemsList.show(engineContext.getSystems(), CreateObjectBrowserListElement<ObjectHandle<System>>);
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