#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"

#include <vector>
#include <iterator>

#include <nlohmann/json.hpp>

namespace zt::core
{
	class ZINET_CORE_API Archive 
	{
	public:

		using Byte = std::byte;
		using BufferT = std::vector<Byte>;

		Archive() ZINET_API_POST = delete;
		Archive(BufferT* newBuffer) ZINET_API_POST : buffer{ newBuffer } {}
		Archive(const Archive& other) ZINET_API_POST = default;
		Archive(Archive&& other) ZINET_API_POST = default;

		Archive& operator = (const Archive& other) ZINET_API_POST = default;
		Archive& operator = (Archive&& other) ZINET_API_POST = default;

		~Archive() ZINET_API_POST = default;

		template<class ObjectT>
		void operator <<(ObjectT& object) ZINET_API_POST 
		{
			constexpr bool requireOperator = requires(ObjectT& object) { object.operator <<(*this); };
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

		OutputArchive() ZINET_API_POST = default;
		OutputArchive(BufferT* newBuffer) ZINET_API_POST : Archive{ newBuffer } {}
		OutputArchive(const OutputArchive& other) ZINET_API_POST = default;
		OutputArchive(OutputArchive&& other) ZINET_API_POST = default;

		OutputArchive& operator = (const OutputArchive& other) ZINET_API_POST = default;
		OutputArchive& operator = (OutputArchive&& other) ZINET_API_POST = default;

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

		InputArchive() ZINET_API_POST = default;
		InputArchive(BufferT* newBuffer, size_t newOffset = 0) ZINET_API_POST : Archive{ newBuffer } { offset = newOffset; }
		InputArchive(const InputArchive& other) ZINET_API_POST = default;
		InputArchive(InputArchive&& other) ZINET_API_POST = default;

		InputArchive& operator = (const InputArchive& other) ZINET_API_POST = default;
		InputArchive& operator = (InputArchive&& other) ZINET_API_POST = default;

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

	class ZINET_CORE_API JsonArchive
	{
	public:

		using BufferT = nlohmann::json;

		JsonArchive() ZINET_API_POST = delete;
		JsonArchive(BufferT* newBuffer) ZINET_API_POST : buffer{ newBuffer } {}
		JsonArchive(const JsonArchive& other) ZINET_API_POST = default;
		JsonArchive(JsonArchive&& other) ZINET_API_POST = default;

		JsonArchive& operator = (const JsonArchive& other) ZINET_API_POST = default;
		JsonArchive& operator = (JsonArchive&& other) ZINET_API_POST = default;

		~JsonArchive() ZINET_API_POST = default;

		template<class ObjectT>
		void serialize(std::string key, ObjectT& object) ZINET_API_POST
		{
			constexpr bool requireSerialize = requires(ObjectT& object) { object.serialize(*this); };
			if constexpr (requireSerialize)
			{
				object.serialize(*this);
			}
			else
			{
				buffer->operator[](key) = object;
			}
		}

		template<class ObjectT>
		void deserialize(std::string key, ObjectT& object) ZINET_API_POST
		{
			constexpr bool requireDeserialize = requires(ObjectT& object) { object.deserialize(*this); };
			if constexpr (requireDeserialize)
			{
				object.deserialize(*this);
			}
			else
			{
				object = buffer->operator[](key);
			}
		}

	protected:

		BufferT* buffer = nullptr;

	};

}