#pragma once

#include "Zinet/Core/ZtCoreConfig.hpp"
#include "Zinet/Core/ZtObject.hpp"
#include "Zinet/Core/ZtLogger.hpp"

#include <filesystem>
#include <memory>

#include <stb_image.h>

namespace zt::core
{
	namespace fs = std::filesystem;

	namespace
	{
		inline constexpr auto stbiFree = [](stbi_uc* ptr) { stbi_image_free(ptr); };
	}

	ZT_REFLECT_CLASS(NO_CONSTRUCTORS, NO_DESTRUCTOR, NO_OPERATORS)
	class ZINET_CORE_API Image : public Object
	{
	protected:

		inline static auto Logger = core::ConsoleLogger::Create("Image");

	public:

		Image() = default;
		Image(const Image& other) = delete;
		Image(Image&& other) = default;

		Image& operator = (const Image& other) = delete;
		Image& operator = (Image&& other) = default;

		~Image() noexcept = default;

		bool loadFromFile(const fs::path& path, std::int32_t expectedComponents) noexcept;

		auto data() const noexcept { return imageData.get(); }
		auto getWidth() const noexcept { return width; }
		auto getHeight() const noexcept { return height; }
		auto getComponents() const noexcept { return components; }

		void destroy() noexcept;

	protected:

		using ImageDataT = std::unique_ptr<stbi_uc, std::remove_const_t<decltype(stbiFree)>>;
		ImageDataT imageData;
		std::int32_t width{};
		std::int32_t height{};
		std::int32_t components{};

	public:
/*GENERATED_CODE_START*/
		static_assert(IsObjectClassInherited); // Class using ZT_REFLECT_CLASS should inherit public from Object class
		const inline static bool RegisterClassResult = RegisterClass<Image>();
		std::unique_ptr<ObjectBase> createCopy() const override { std::unique_ptr<ObjectBase> result = createCopyInternal<Image>(); *result = *this; return result; }
		
		class ClassInfo : public zt::core::ClassInfoBase
		{
		public:
		
			std::string_view getClassName() const override { return "Image"; }
		};
		const zt::core::ClassInfoBase* getClassInfo() const override { static ClassInfo classInfo; return &classInfo; }
		
		
/*GENERATED_CODE_END*/
	};

}