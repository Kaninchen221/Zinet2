#pragma once

#include <array>
#include <functional>
#include <concepts>
#include <type_traits>

#include "Zinet/Core/ZtDebug.hpp"
#include <map>

#ifdef ZINET_COLLECT_ALLOCATION_DEBUG_DATA

template<typename Operation>
concept MustBePlusOrMinus = std::same_as<std::plus<>, Operation> || std::same_as<std::minus<>, Operation>;

// If you want to use it then include it in main.cpp file before any include and define ZINET_COLLECT_ALLOCATION_DEBUG_DATA
class ZinetCollectAllocationDebugData
{
public:

	using BigUInt = unsigned long long int;
	using BigInt = signed long long int;

	struct Pointer
	{
		void* rawPointer = nullptr;
		size_t size = 0u;
	};

	template<MustBePlusOrMinus Operation>
	static void CalculateAllocatedBlocksCount();

// 	template<MustBePlusOrMinus Operation>
// 	static void CalculateAllocatedSizeCountStatistics(BigUInt size);

	static void* NewPointer(std::size_t size);

	static void Delete(void* pointerToFree);

protected:

	inline static std::array<Pointer, 100000> Pointers{ nullptr };

	inline static BigUInt AllocatedBlocksCount = 0u;
	inline static BigUInt HighestAllocatedBlocksCount = 0u;

// 	inline static std::map<BigUInt, BigUInt> AllocatedSizeCountStatistics;

	const inline static constexpr bool GatherData = true;

	const inline static constexpr bool PrintInfo = true;

};

template<MustBePlusOrMinus Operation>
void ZinetCollectAllocationDebugData::CalculateAllocatedBlocksCount()
{
	AllocatedBlocksCount = Operation{}(AllocatedBlocksCount, 1);
	if constexpr (std::is_same_v<Operation, std::plus<>>)
	{
		if (HighestAllocatedBlocksCount < AllocatedBlocksCount)
			HighestAllocatedBlocksCount = AllocatedBlocksCount;
	}
	else if constexpr (std::is_same_v<Operation, std::minus<>>)
	{
		if (AllocatedBlocksCount > 0)
			--AllocatedBlocksCount;
		else
			zt::Ensure(false); // Free data that is not registered
	}
}
// 
// template<MustBePlusOrMinus Operation>
// void ZinetCollectAllocationDebugData::CalculateAllocatedSizeCountStatistics(BigUInt size)
// {
// 	if constexpr (std::is_same_v<Operation, std::plus<>>)
// 	{
// 		AllocatedSizeCountStatistics[size]++;
// 	}
// 	else if constexpr (std::is_same_v<Operation, std::minus<>>)
// 	{
// 		std::pair<BigUInt, BigUInt>& element = AllocatedSizeCountStatistics.at(size);
// 		if (element.second > 0)
// 			--AllocatedBlocksCount;
// 		else
// 			zt::Ensure(false);
// 	}
// }

void* ZinetCollectAllocationDebugData::NewPointer(std::size_t size)
{
	if constexpr (GatherData)
	{
		void* newPointer = std::malloc(size);
		for (Pointer& pointer : Pointers)
		{
			if (pointer.rawPointer == nullptr)
			{
				pointer.rawPointer = newPointer;
				pointer.size = size;

				CalculateAllocatedBlocksCount<std::plus<>>();

				return newPointer;
			}
		}

		CalculateAllocatedBlocksCount<std::plus<>>();

		return newPointer; // Out of size
	}
	else
	{
		return std::malloc(size);
	}
}

void ZinetCollectAllocationDebugData::Delete(void* pointerToFree)
{
	if constexpr (GatherData)
	{
		for (Pointer& pointer : Pointers)
		{
			if (pointer.rawPointer == pointerToFree)
			{
				pointer = Pointer{};

				if (AllocatedBlocksCount > 0)
				{
					CalculateAllocatedBlocksCount<std::minus<>>();
				}

				std::free(pointerToFree);
				return;
			}
		}

		if (AllocatedBlocksCount > 0)
		{
			CalculateAllocatedBlocksCount<std::minus<>>();
		}

		std::free(pointerToFree); // Not registered pointer
		return;
	}
	else
	{
		std::free(pointerToFree);
	}
}

void* operator new (std::size_t size)
{
	return ZinetCollectAllocationDebugData::NewPointer(size);
}

void operator delete(void* pointerToFree) noexcept
{
	return ZinetCollectAllocationDebugData::Delete(pointerToFree);
}

void* operator new[](std::size_t size)
{
	return ZinetCollectAllocationDebugData::NewPointer(size);
}

void operator delete[](void* pointerToFree) noexcept
{
	return ZinetCollectAllocationDebugData::Delete(pointerToFree);
}

#endif