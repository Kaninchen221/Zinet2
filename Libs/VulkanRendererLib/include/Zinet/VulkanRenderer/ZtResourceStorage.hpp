#pragma once

#include "Zinet/VulkanRenderer/ZtVulkanRendererConfig.hpp"
#include "Zinet/VulkanRenderer/ZtVulkanObject.hpp"
#include "Zinet/VulkanRenderer/ZtRendererContext.hpp"

#include "Zinet/Core/ZtLogger.hpp"

#include "Zinet/Core/Assets/ZtAssetsStorage.hpp"

#include "Zinet/Core/ECS/ZtTypeLessVector.hpp"

#include <vulkan/vulkan.h>

namespace zt::vulkan_renderer
{
	class RendererContext;

	template<class ResourceT, class AssetT>
	struct ResourceHandle
	{
		using AssetHandleT = core::ConstAssetHandle<AssetT>;

		AssetHandleT assetHandle;
		std::optional<ResourceT> resource;
	};

	class ZINET_VULKAN_RENDERER_API ResourceStorage
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("zt::vulkan_renderer::ResourceStorage");

		using Resources = std::vector<core::ecs::TypeLessVector>;
		using Requests = std::vector<std::function<bool(const RendererContext&)>>;

	public:

		ResourceStorage() noexcept = default;
		ResourceStorage(const ResourceStorage& other) noexcept = default;
		ResourceStorage(ResourceStorage&& other) noexcept = default;
		~ResourceStorage() noexcept = default;

		ResourceStorage& operator = (const ResourceStorage& other) noexcept = default;
		ResourceStorage& operator = (ResourceStorage&& other) noexcept = default;

		template<class ResourceT, class AssetT>
		const ResourceT* request(core::ConstAssetHandle<AssetT> assetHandle);

		bool createResources(const RendererContext& rendererContext);

	protected:

		template<class ResourceT, class AssetT>
		ResourceHandle<ResourceT, AssetT>* findResourceHandle(core::ConstAssetHandle<AssetT> assetHandle) noexcept;

		template<class ResourceT, class AssetT>
		size_t addResourceHandle(core::ConstAssetHandle<AssetT> assetHandle) noexcept;

		Resources resources;
		Requests requests;
	};

	template<class ResourceT, class AssetT>
	const ResourceT* vulkan_renderer::ResourceStorage::request(core::ConstAssetHandle<AssetT> assetHandle)
	{
		if (!assetHandle)
		{
			Logger->error("Asset handle is invalid");
			return {};
		}
		
		AssetT& asset = *assetHandle.get();

		constexpr bool hasCreateFunc = requires { { asset.createResource(RendererContext{}) } -> std::same_as<std::optional<ResourceT>>; };

		static_assert(hasCreateFunc,
			"AssetT must have a createResource method that will return an object of class ResourceT from the asset"
			" and takes RendererContext& as a param");

		auto resourceHandle = findResourceHandle<ResourceT, AssetT>(assetHandle);
		if (resourceHandle && resourceHandle->resource.has_value())
		{
			return &resourceHandle->resource.value();
		}

		if (resourceHandle)
			return {};

		// Add new resource handle
		addResourceHandle<ResourceT, AssetT>(assetHandle);

		// Add new resource request
		auto newRequest = [self = this, assetHandle = assetHandle]
		(const RendererContext& rendererContext) -> bool
		{
			if (!assetHandle)
				return false;

			if (!assetHandle->isLoaded())
				return false;

			using ResourceHandleT = ResourceHandle<ResourceT, AssetT>;
			auto resource = assetHandle->createResource(rendererContext);
			if (!resource)
				return false;

			auto resourceHandle = self->findResourceHandle<ResourceT, AssetT>(assetHandle);
			resourceHandle->resource = std::move(resource);

			return true;
		};
		requests.push_back(newRequest);

		return {};
	}

	template<class ResourceT, class AssetT>
	ResourceHandle<ResourceT, AssetT>* vulkan_renderer::ResourceStorage::findResourceHandle(core::ConstAssetHandle<AssetT> assetHandle) noexcept
	{
		using ResourceHandleT = ResourceHandle<ResourceT, AssetT>;
		for (auto& handles : resources)
		{
			if (handles.hasType<ResourceHandleT>())
			{
				for (auto handleRawPtr : handles)
				{
					auto& handle = *reinterpret_cast<ResourceHandleT*>(handleRawPtr);
					if (handle.assetHandle == assetHandle)
						return &handle;
				}
			}
		}

		return {};
	}

	template<class ResourceT, class AssetT>
	size_t ResourceStorage::addResourceHandle(core::ConstAssetHandle<AssetT> assetHandle) noexcept
	{
		using ResourceHandleT = ResourceHandle<ResourceT, AssetT>;
		for (auto& handles : resources)
		{
			if (handles.hasType<ResourceHandleT>())
			{
				return handles.add(ResourceHandleT{ .assetHandle = assetHandle });
			}
		}

		auto& handles = resources.emplace_back(core::ecs::TypeLessVector::Create<ResourceHandleT>());
		return handles.add(ResourceHandleT{ .assetHandle = assetHandle });
	}
}