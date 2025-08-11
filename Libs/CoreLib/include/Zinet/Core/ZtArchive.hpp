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

		Archive() = delete;
		Archive(BufferT* newBuffer) noexcept : buffer{ newBuffer } {}
		Archive(const Archive& other) noexcept = delete;
		Archive(Archive&& other) noexcept = default;

		Archive& operator = (const Archive& other) noexcept = default;
		Archive& operator = (Archive&& other) noexcept = default;

		~Archive() noexcept = default;

		template<class ObjectT>
		void operator <<(ObjectT& object) 
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

		virtual void internal([[maybe_unused]] Byte* objectAddress, [[maybe_unused]] size_t objectSize) {}

		BufferT* buffer = nullptr;
	};

	class ZINET_CORE_API OutputArchive : public Archive 
	{
	public:

		OutputArchive() noexcept = default;
		OutputArchive(BufferT* newBuffer) noexcept : Archive{ newBuffer } {}
		OutputArchive(const OutputArchive& other) noexcept = delete;
		OutputArchive(OutputArchive&& other) noexcept = default;

		OutputArchive& operator = (const OutputArchive& other) noexcept = delete;
		OutputArchive& operator = (OutputArchive&& other) noexcept = default;

		~OutputArchive() noexcept = default;

	protected:

		void internal(Byte* objectAddress, size_t objectSize) override
		{
			buffer->resize(buffer->size() + objectSize);

			auto dest = buffer->begin() + (buffer->size() - objectSize);
			std::memcpy(std::to_address(dest), objectAddress, objectSize);
		}
	};

	class ZINET_CORE_API InputArchive : public Archive
	{
	public:

		InputArchive() noexcept = default;
		InputArchive(BufferT* newBuffer, size_t newOffset = 0) noexcept : Archive{ newBuffer } { offset = newOffset; }
		InputArchive(const InputArchive& other) noexcept = delete;
		InputArchive(InputArchive&& other) noexcept = default;

		InputArchive& operator = (const InputArchive& other) noexcept = delete;
		InputArchive& operator = (InputArchive&& other) noexcept = default;

		~InputArchive() noexcept = default;

	protected:

		void internal(Byte* objectAddress, size_t objectSize) override
		{
			auto source = buffer->begin() + offset;
			std::memcpy(objectAddress, std::to_address(source), objectSize);
			offset += objectSize;
		}

		size_t offset = 0;
	};

	using Json = nlohmann::json;

	class ZINET_CORE_API JsonArchive
	{
	public:

		using BufferT = Json;

		JsonArchive() noexcept = delete;
		JsonArchive(BufferT* newBuffer) noexcept : buffer{ newBuffer } {}
		JsonArchive(const JsonArchive& other) noexcept = delete;
		JsonArchive(JsonArchive&& other) noexcept = default;

		JsonArchive& operator = (const JsonArchive& other) noexcept = delete;
		JsonArchive& operator = (JsonArchive&& other) noexcept = default;

		~JsonArchive() noexcept = default;

		template<class ObjectT>
		void serialize(std::string key, ObjectT&& object)
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
		void deserialize(std::string key, ObjectT& object)
		{
			constexpr bool requireDeserialize = requires(ObjectT& object) { object.deserialize(*this); };
			if constexpr (requireDeserialize)
			{
				object.deserialize(*this);
			}
			else
			{
				auto findIt = buffer->find(key);
				if (findIt != buffer->end())
					object = *findIt;
			}
		}

		auto* getBuffer() noexcept { return buffer; }

	protected:

		BufferT* buffer = nullptr;

	};

}