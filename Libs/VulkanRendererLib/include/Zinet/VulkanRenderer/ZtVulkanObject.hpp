#pragma once

#include <limits>
#include <cstdint>
#include <utility>

#include "Zinet/Core/ZtLogger.hpp"
#include "Zinet/Core/ZtDebug.hpp"

namespace zt::vulkan_renderer
{
	template<class VulkanT, size_t HandlesCount = 1>
	class VulkanObject
	{
	public:

		// TODO: Not "std::numeric_limits<std::uint32_t>::max();" because MSVC see the "max()" part as the "max()" macro
		inline static constexpr std::uint32_t InvalidIndex = UINT32_MAX;

		using VulcanType = VulkanT;
		using HandleType = std::array<VulkanT, HandlesCount>;

		VulkanObject(VulcanType newHandle) noexcept
			: handles({ newHandle }) {}

		VulkanObject() = delete;
		VulkanObject(const VulkanObject& other) = delete;
		VulkanObject(VulkanObject&& other) noexcept { *this = std::move(other); }
		~VulkanObject() noexcept;

		VulkanObject& operator = (const VulkanObject& other) = delete;
		VulkanObject& operator = (VulkanObject&& other) noexcept;

		const VulcanType& get() const noexcept { return handles.front(); }
		VulcanType& get() noexcept { return handles.front(); }

		bool isValid() const noexcept { return handles.front() != nullptr; }

		operator bool() const noexcept { return isValid(); }

	protected:

		void invalidateInternal() noexcept { handles.fill(nullptr); }

	private:

		HandleType handles;

	};

	// TODO: Upper case
	inline void invalidateAll(auto&& container) noexcept
	{
		for (auto&& element : container)
		{
			element.invalidate();
		}
	}

	// TODO: Upper case
	template<class ...ArgsT>
	inline void destroyAll(auto&& container, ArgsT&& ... args)
	{
		for (auto&& element : container)
		{
			element.destroy(std::forward<ArgsT>(args)...);
		}
	}

	template<class ResultContainerT>
	inline ResultContainerT VulkanObjectsToVkObjects(auto&& container)
	{
		ResultContainerT resultContainer;
		for (auto&& vulkanObject : container)
		{
			resultContainer.push_back(vulkanObject.get());
		}

		return resultContainer;
	}

	template<class VulcanType, size_t HandlesCount>
	VulkanObject<VulcanType, HandlesCount>& VulkanObject<VulcanType, HandlesCount>::operator=(VulkanObject&& other) noexcept
	{
		if (isValid())
		{
			auto Logger = core::ConsoleLogger::Create("VulkanObject");
			Logger->error("Object must be manually destroyed/invalidated before move call");
		}

		handles = other.handles;
		other.handles.fill(nullptr);
		return *this;
	}

	template<class VulcanType, size_t HandlesCount>
	VulkanObject<VulcanType, HandlesCount>::~VulkanObject() noexcept
	{
		if (isValid())
		{
			Ensure(false);
			try 
			{
				auto Logger = core::ConsoleLogger::Create("VulkanObject");
				Logger->error("Object must be manually destroyed/invalidated before destructor call");
			}
			catch ([[maybe_unused]] std::exception& ex)
			{
				TerminateDebug();
			}
		}
	}

}