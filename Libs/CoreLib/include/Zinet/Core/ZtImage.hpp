#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"
#include "Zinet/Core/ZtObject.hpp"
#include "Zinet/Core/ZtLogger.hpp"

#include <filesystem>
#include <memory>

#include <stb_image.h>

namespace zt::core
{
	namespace
	{
		namespace fs = std::filesystem;
	}

	inline constexpr auto stbiFree = [](stbi_uc* ptr) { std::free(ptr); };

	class ZINET_CORE_API Image : public Object
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("zt::core::Image");

	public:

		static_assert(sizeof(std::byte) == sizeof(stbi_uc));
		using Data = std::vector<std::byte>;

		Image() = default;
		Image(const Image& other) = delete;
		Image(Image&& other) { *this = std::move(other); }
		~Image() ZINET_API_POST = default;

		Image& operator = (const Image& other) = delete;
		Image& operator = (Image&& other);

		bool loadFromFile(const fs::path& path, int32_t expectedComponents) ZINET_API_POST;

		bool loadFromData(const Data& data, int32_t expectedComponents) ZINET_API_POST;

		auto data() const ZINET_API_POST { return imageData.get(); }
		auto getWidth() const ZINET_API_POST { return width; }
		auto getHeight() const ZINET_API_POST { return height; }
		auto getComponents() const ZINET_API_POST { return components; }
		auto getSize() const ZINET_API_POST { return width * height * components; }

		void destroy() ZINET_API_POST;

	protected:

		using ImageDataT = std::unique_ptr<stbi_uc, std::remove_const_t<decltype(stbiFree)>>;
		ImageDataT imageData;
		std::int32_t width{};
		std::int32_t height{};
		std::int32_t components{};
	};

}