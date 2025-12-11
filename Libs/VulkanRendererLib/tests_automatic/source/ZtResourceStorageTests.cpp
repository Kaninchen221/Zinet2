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
			inline static auto Logger = core::ConsoleLogger::Create("zt::vulkan_renderer::tests::ResourceStorageTests::Resource");

		public:

			Resource(int value) : value{ value } {}
			Resource(const Resource& other) = delete;
			Resource(Resource&& other) noexcept = default;
			~Resource() 
			{ 
				if (errorCallback)
					errorCallback(); 
			}

			Resource& operator = (const Resource& other) = delete;
			Resource& operator = (Resource&& other) noexcept = default;

			auto getValue() const noexcept { return value; }

			// Typical destroy method
			void destroy(const VMA&) noexcept { errorCallback = {}; }

			// Destroy method for ResourceStorage
			void destroy(const RendererContext& rendererContext) noexcept { destroy(rendererContext.getVMA()); }

		private:

			int value = 0;

			std::function<void()> errorCallback = []()
			{
				Logger->error("You didn't call destroy for Resource class object");
			};

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
		ASSERT_EQ(resource->getValue(), expectedValue);

		// Resource storage must be cleaned before destructor call to avoid errors related
		// to not destroyed vulkan resources
		resourceStorage.clear(constRendererContext);
	}
}