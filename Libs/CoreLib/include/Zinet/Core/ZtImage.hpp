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

	ZT_REFLECT_CLASS(NO_CONSTRUCTORS, NO_DESTRUCTOR, NO_OPERATORS)
	class ZINET_CORE_API Image : public Object
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("Image");

	public:

		Image() = default;
		Image(const Image& other) = delete;
		Image(Image&& other) { *this = std::move(other); }

		Image& operator = (const Image& other) = delete;
		Image& operator = (Image&& other);

		~Image() ZINET_API_POST = default;

		bool loadFromFile(const fs::path& path, int32_t expectedComponents) ZINET_API_POST;

		using Data = std::vector<uint8_t>;
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