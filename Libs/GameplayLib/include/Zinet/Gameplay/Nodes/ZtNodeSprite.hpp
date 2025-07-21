#pragma once

#include "Zinet/Gameplay/ZtGameplayConfig.hpp"
#include "Zinet/Gameplay/Nodes/ZtNode2D.hpp"
#include "Zinet/Gameplay/Assets/ZtAssetTexture.hpp"

#include "Zinet/Core/ZtLogger.hpp"

#include "Zinet/VulkanRenderer/ZtTransform.hpp"

#include <imgui.h>

namespace zt::gameplay
{
	template<std::derived_from<core::assets::Asset> AssetT>
	struct EditorAssetProperty
	{
		inline static auto Logger = core::ConsoleLogger::Create("zt::gameplay::EditorAssetProperty");

		using AssetHandleT = core::assets::AssetHandle<AssetT>;

		// Config
		std::string propertyName = "PropertyName";

		// Data
		core::assets::AssetHandle<AssetT> assetHandle;

		auto operator = (AssetHandleT otherAssetHandle) ZINET_API_POST { assetHandle = otherAssetHandle; }

		void show() ZINET_API_POST;
	};

	template<std::derived_from<core::assets::Asset> AssetT>
	void gameplay::EditorAssetProperty<AssetT>::show() ZINET_API_POST
	{
		auto assetDragAndDrop = [&assetHandle = assetHandle]()
		{
			// Target for drag and drop
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(AssetPayloadType))
				{
					core::assets::Asset* asset = nullptr;
					std::memcpy(&asset, payload->Data, sizeof(AssetT*));
					AssetT* castedAsset = dynamic_cast<AssetT*>(asset);
					if (castedAsset)
					{
						assetHandle = core::assets::AssetHandle<AssetT>{ castedAsset };
						Logger->info("Set a new asset");
					}
					else
					{
						Logger->info("Not a compatible type");
					}
				}

				ImGui::EndDragDropTarget();
			}
		};

		auto invalidate = [&assetHandle = assetHandle]()
		{
			ImVec2 prevSize = ImGui::GetItemRectSize();
			ImGui::SameLine();
			ImVec2 size{ prevSize.y, prevSize.y };
			if (ImGui::SmallButton("X"))
			{
				assetHandle.invalidate();
			}

			if (ImGui::BeginItemTooltip())
			{
				ImGui::Text("Invalidate Asset handle");
				ImGui::EndTooltip();
			}
		};

		const ImGuiTableFlags tableFlags = {};
		if (ImGui::BeginTable("propertyTable", 3 /*columns*/, tableFlags))
		{
			ImGui::TableSetupColumn("Property Name", ImGuiTableColumnFlags_WidthFixed);
			ImGui::TableSetupColumn("Asset Name", ImGuiTableFlags_SizingStretchProp);
			ImGui::TableSetupColumn("Invalidate Button", ImGuiTableColumnFlags_WidthFixed);

			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text(fmt::format("{}:", propertyName).c_str());

			if (assetHandle)
			{
				ImGui::TableNextColumn();
				auto assetName = assetHandle->metaData.value("fileName", "fileName");
				ImGui::Text(assetName.c_str());
			}
			else
			{
				ImGui::TableNextColumn();
				ImGui::Text("Invalid asset handle");
			}

			ImGui::TableNextColumn();
			invalidate();

			ImGui::EndTable();
			assetDragAndDrop();
		}

		if (assetHandle)
		{
			const std::string isLoaded = fmt::format("Is loaded: {}", assetHandle->isLoaded());
			ImGui::Text(isLoaded.c_str());
		}
	}

	class ZINET_GAMEPLAY_API NodeSprite : public Node2D
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("zt::gameplay::NodeSprite");

	public:

		NodeSprite() ZINET_API_POST = delete;
		NodeSprite(const NodeNameView& newName) ZINET_API_POST : Node2D{ newName } {}
		NodeSprite(const NodeSprite& other) ZINET_API_POST = default;
		NodeSprite(NodeSprite&& other) ZINET_API_POST = default;
		~NodeSprite() ZINET_API_POST = default;

		NodeSprite& operator = (const NodeSprite& other) ZINET_API_POST = default;
		NodeSprite& operator = (NodeSprite&& other) ZINET_API_POST = default;

		void imGuiNodeInspect() ZINET_API_POST override;

		EditorAssetProperty<assets::AssetTexture> texture{ "Texture" };

	};
}