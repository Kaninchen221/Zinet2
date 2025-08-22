#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"
#include "Zinet/Core/ZtImGui.hpp"
#include "Zinet/Core/ZtLogger.hpp"
#include "Zinet/Core/Assets/ZtAsset.hpp"

#include "Zinet/Gameplay/ZtEngineContext.hpp"

#include <concepts>

namespace zt::gameplay
{
	template<std::derived_from<core::Asset> AssetT>
	class AssetProperty
	{
	public:

		AssetProperty() = default;
		AssetProperty(const std::string_view name)
			: propertyName(name) {
		}
		AssetProperty(const AssetProperty& other) = default;
		AssetProperty(AssetProperty&& other) noexcept = default;
		~AssetProperty() noexcept = default;

		AssetProperty& operator = (const AssetProperty& other) = default;
		AssetProperty& operator = (AssetProperty&& other) noexcept = default;

		inline static auto Logger = core::ConsoleLogger::Create("zt::gameplay::AssetProperty");

		using AssetHandleT = typename core::AssetHandle<AssetT>;

		AssetProperty& operator = (AssetHandleT otherAssetHandle) noexcept { assetHandle = otherAssetHandle; return *this; }

		AssetT* operator->() noexcept { return assetHandle.operator->(); }

		operator bool() noexcept { return assetHandle; }

		bool serialize(core::JsonArchive& archive);

		bool deserialize(core::JsonArchive& archive);

		void show();
	
		void setPropertyName(const std::string_view newPropertyName) { propertyName = newPropertyName; }
		const std::string_view getPropertyName() const { return propertyName; }

		void setAssetHandle(AssetHandleT newAssetHandle) { assetHandle = newAssetHandle; }
		const AssetHandleT& getAssetHandle() const { return assetHandle; }

	protected:

		std::string propertyName = "Default Name";

		// TODO: OnChanged
		core::AssetHandle<AssetT> assetHandle;
	};

	template<std::derived_from<core::Asset> AssetT>
	bool AssetProperty<AssetT>::serialize(core::JsonArchive& archive)
	{
		if (assetHandle)
			archive.serialize(propertyName, assetHandle->getMetaData().value("fileRelativePath", "invalid relative path"));

		return true;
	}

	template<std::derived_from<core::Asset> AssetT>
	bool AssetProperty<AssetT>::deserialize(core::JsonArchive& archive)
	{
		std::string assetKey;
		archive.deserialize(propertyName, assetKey);
		if (!assetKey.empty())
		{
			auto& engineContext = EngineContext::Get();
			assetHandle = engineContext.getAssetsStorage().getAs<AssetT>(assetKey);
		}

		return true;
	}

	template<std::derived_from<core::Asset> AssetT>
	void AssetProperty<AssetT>::show()
	{
		ImGui::PushID(this);

		auto assetDragAndDrop = [&assetHandle = assetHandle]()
		{
			// Target for drag and drop
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(ZinetImGuiPayloadType))
				{
					core::ObjectRefCounter* objRefCounter{};
					void* destiny = &objRefCounter;
					std::memcpy(destiny, payload->Data, sizeof(AssetT*));
					AssetT* castedAsset = dynamic_cast<AssetT*>(objRefCounter->get());
					if (castedAsset)
					{
						assetHandle = core::AssetHandle<AssetT>{ objRefCounter };
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
				ImGui::TextCStr("Invalidate Asset handle");
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
			ImGui::Text(propertyName);

			if (assetHandle)
			{
				ImGui::TableNextColumn();
				auto assetName = assetHandle->getMetaData().value("fileName", "fileName");
				ImGui::Text(assetName);
			}
			else
			{
				ImGui::TableNextColumn();
				ImGui::TextCStr("Invalid asset handle");
			}

			ImGui::TableNextColumn();
			invalidate();

			ImGui::EndTable();
			assetDragAndDrop();
		}

		if (assetHandle)
		{
			ImGui::TextFMT("Is loaded: {}", assetHandle->isLoaded());
		}

		ImGui::PopID();
		ImGui::Separator();
	}

}