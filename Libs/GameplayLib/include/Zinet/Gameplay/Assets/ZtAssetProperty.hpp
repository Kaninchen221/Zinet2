#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"
#include "Zinet/Core/ZtImGui.hpp"
#include "Zinet/Core/ZtLogger.hpp"
#include "Zinet/Core/Assets/ZtAsset.hpp"
#include "Zinet/Core/ZtConstants.hpp"

#include "Zinet/Core/Assets/ZtAssetStorage.hpp"

#include <concepts>

namespace zt::gameplay::asset
{
	template<std::derived_from<core::Asset> AssetT, class Static = core::Static::No>
	class Property
	{
	public:

		using AssetHandleT = typename core::AssetHandle<AssetT>;

		using IsStatic = Static;

		Property() = default;
		Property(const std::string_view name)
			: propertyName(name) {
		}
		Property(const Property& other) = default;
		Property(Property&& other) noexcept = default;
		~Property() noexcept 
		{
			// Static asset properties should not release their asset handle on destruction because it's already destroyed by the assets storage
			if constexpr (IsStatic{})
				assetHandle.release();
		}

		Property& operator = (const Property& other) = default;
		Property& operator = (Property&& other) noexcept = default;

		inline static auto Logger = core::ConsoleLogger::Create("zt::gameplay::Property");

		Property& operator = (AssetHandleT otherAssetHandle) noexcept { assetHandle = otherAssetHandle; return *this; }

		AssetT* operator->() noexcept 
		{ 
#		if ZINET_DEBUG
			if (!assetHandle)
			{
				Logger->critical("Asset handle is invalid");
				Ensure(false);
			}
#		endif

			return assetHandle.operator->(); 
		}

		AssetT* get() noexcept { return assetHandle.get(); }

		bool isValid() const noexcept { return assetHandle.isValid(); }

		operator bool() const noexcept { return isValid(); }

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

	template<std::derived_from<core::Asset> AssetT, class Static>
	void Property<AssetT, Static>::show()
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
					std::memcpy(destiny, payload->Data, sizeof(core::ObjectRefCounter*));
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
				auto assetName = assetHandle->getMetaData().value("fileNameExt", "fileNameExt");
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