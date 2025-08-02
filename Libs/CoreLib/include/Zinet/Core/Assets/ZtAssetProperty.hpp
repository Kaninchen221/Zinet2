#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"

#include <concepts>

#include "Zinet/Core/Assets/ZtAsset.hpp"

#include <imgui.h>

namespace zt::core::assets
{
	// TODO: Serialization
	template<std::derived_from<Asset> AssetT>
	struct AssetProperty
	{
		inline static auto Logger = ConsoleLogger::Create("zt::gameplay::EditorAssetProperty");

		using AssetHandleT = assets::AssetHandle<AssetT>;

		// Config
		std::string propertyName = "PropertyName";

		// Data
		// TODO: OnChanged
		assets::AssetHandle<AssetT> assetHandle;

		auto operator = (AssetHandleT otherAssetHandle) ZINET_API_POST { assetHandle = otherAssetHandle; }

		auto* operator->() ZINET_API_POST { return assetHandle.operator->(); }

		operator bool() ZINET_API_POST { return assetHandle; }

		void show() ZINET_API_POST;
	};

	template<std::derived_from<Asset> AssetT>
	void AssetProperty<AssetT>::show() ZINET_API_POST
	{
		ImGui::PushID(this);

		auto assetDragAndDrop = [&assetHandle = assetHandle]()
		{
			// Target for drag and drop
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(ZinetImGuiPayloadType))
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

		const ImGuiTableFlags tableFlags = {};// ImGuiTableFlags::ImGuiTableFlags_Borders;
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

		ImGui::PopID();
		ImGui::Separator();
	}

}