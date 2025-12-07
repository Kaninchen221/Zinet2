#pragma once

#include "Zinet/VulkanRenderer/ZtResourceStorage.hpp"

#include "Zinet/Core/Assets/ZtAssetStorage.hpp"

#include <gtest/gtest.h>

#include <type_traits>

namespace zt::vulkan_renderer::tests
{
	class ResourceStorageTests : public ::testing::Test
	{
	protected:

		void SetUp() override
		{
		}

		void TearDown() override
		{
			objectRefCounter.destroy();
		}

		static_assert(std::is_default_constructible_v<ResourceStorage>);
		static_assert(std::is_copy_constructible_v<ResourceStorage>);
		static_assert(std::is_copy_assignable_v<ResourceStorage>);
		static_assert(std::is_move_constructible_v<ResourceStorage>);
		static_assert(std::is_move_assignable_v<ResourceStorage>);
		static_assert(std::is_destructible_v<ResourceStorage>);

		class Resource 
		{
		public:

			int value{};
		};

		class Asset : public core::Asset
		{
		public:

			Asset()
			{
				loaded = true;
			}

			std::optional<Resource> createResource(const RendererContext&) 
			{ 
				Resource resource{ expectedValue };
				return resource;
			}

		};

		auto createConstAssetHandle()
		{
			objectRefCounter.create<Asset>("Asset");
			auto assetHandle = core::ConstAssetHandle<Asset>{ &objectRefCounter };
			return std::move(assetHandle);
		}

		ResourceStorage resourceStorage;
		inline static const int expectedValue = 50;

	private:

		core::ObjectRefCounter objectRefCounter;
	};

	TEST_F(ResourceStorageTests, RequestTest)
	{
		core::ConstAssetHandle<Asset> assetHandle = createConstAssetHandle();

		const Resource* resource = resourceStorage.request<Resource, Asset>(assetHandle);
		ASSERT_FALSE(resource);

		RendererContext rendererContext;
		auto& constRendererContext = rendererContext;

		const bool createResourcesResult = resourceStorage.createResources(constRendererContext);
		ASSERT_TRUE(createResourcesResult);

		resource = resourceStorage.request<Resource, Asset>(assetHandle);
		ASSERT_TRUE(resource);
		ASSERT_EQ(resource->value, expectedValue);
	}
}