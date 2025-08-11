#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"
#include "Zinet/Core/ZtObject.hpp"
#include "Zinet/Core/ZtLogger.hpp"
#include "Zinet/Core/ZtPaths.hpp"

#include <filesystem>
#include <memory>

#include <stb_image.h>

namespace zt::core
{
	class ZINET_CORE_API Image : public Object
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("zt::core::Image");

	public:

		static_assert(sizeof(std::byte) == sizeof(stbi_uc));
		using Data = std::vector<std::byte>;

		Image() noexcept = default;
		Image(const Image& other) = delete;
		Image(Image&& other) noexcept { *this = std::move(other); }
		~Image() noexcept { destroy(); }

		Image& operator = (const Image& other) = delete;
		Image& operator = (Image&& other) noexcept;

		bool loadFromFile(const Path& path, int32_t expectedComponents);

		bool loadFromData(const Data& rawData, int32_t expectedComponents);

		auto getData() const noexcept { return data; }
		auto getWidth() const noexcept { return width; }
		auto getHeight() const noexcept { return height; }
		auto getComponents() const noexcept { return components; }
		auto getSize() const noexcept { return width * height * components; }

		void destroy() noexcept;

	protected:

		stbi_uc* data{};
		std::int32_t width{};
		std::int32_t height{};
		std::int32_t components{};
	};

}