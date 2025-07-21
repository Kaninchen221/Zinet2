#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"

#include <vector>
#include <iterator>

namespace zt::core
{
	class ZINET_CORE_API Archive 
	{
	public:

		using Byte = std::byte;
		using BufferT = std::vector<Byte>;

		Archive() = delete;
		Archive(BufferT* newBuffer) : buffer{ newBuffer } {}
		Archive(const Archive& other) = default;
		Archive(Archive&& other) = default;

		Archive& operator = (const Archive& other) = default;
		Archive& operator = (Archive&& other) = default;

		~Archive() ZINET_API_POST = default;

		template<class ObjectT>
		void operator <<(ObjectT& object) ZINET_API_POST 
		{
			constexpr bool requireOperator = requires(ObjectT & object) { object.operator <<(*this); };
			if constexpr (requireOperator)
			{
				object.operator <<(*this);
			}
			else
			{
				internal(reinterpret_cast<Byte*>(&object), sizeof(std::decay_t<decltype(object)>));
			}
		}

	protected:

		virtual void internal([[maybe_unused]] Byte* objectAddress, [[maybe_unused]] size_t objectSize) ZINET_API_POST {}

		BufferT* buffer = nullptr;
	};

	class ZINET_CORE_API OutputArchive : public Archive 
	{
	public:

		OutputArchive() = default;
		OutputArchive(BufferT* newBuffer) : Archive{ newBuffer } {}
		OutputArchive(const OutputArchive& other) = default;
		OutputArchive(OutputArchive&& other) = default;

		OutputArchive& operator = (const OutputArchive& other) = default;
		OutputArchive& operator = (OutputArchive&& other) = default;

		~OutputArchive() ZINET_API_POST = default;

	protected:

		void internal(Byte* objectAddress, size_t objectSize) ZINET_API_POST override
		{
			buffer->resize(buffer->size() + objectSize);

			auto dest = buffer->begin() + (buffer->size() - objectSize);
			std::memcpy(std::to_address(dest), objectAddress, objectSize);
		}
	};

	class ZINET_CORE_API InputArchive : public Archive
	{
	public:

		InputArchive() = default;
		InputArchive(BufferT* newBuffer, size_t newOffset = 0) : Archive{ newBuffer } { offset = newOffset; }
		InputArchive(const InputArchive& other) = default;
		InputArchive(InputArchive&& other) = default;

		InputArchive& operator = (const InputArchive& other) = default;
		InputArchive& operator = (InputArchive&& other) = default;

		~InputArchive() ZINET_API_POST = default;

	protected:

		void internal(Byte* objectAddress, size_t objectSize) ZINET_API_POST override
		{
			auto source = buffer->begin() + offset;
			std::memcpy(objectAddress, std::to_address(source), objectSize);
			offset += objectSize;
		}

		size_t offset = 0;
	};
}