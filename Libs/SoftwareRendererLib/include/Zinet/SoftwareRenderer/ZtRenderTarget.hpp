#pragma once

#include "Zinet/SoftwareRenderer/ZtSoftwareRendererConfig.hpp"

#include "Zinet/Core/ZtLogger.hpp"
#include "Zinet/Core/Reflection/ZtReflection.hpp"
#include "Zinet/Core/ZtClock.hpp"
#include "Zinet/Core/ZtCoreConfig.hpp"

#include "Zinet/Math/ZtVecTypes.hpp"

#include <filesystem>

namespace zt::software_renderer
{
	struct ZINET_SOFTWARE_RENDERER_API Pixel
	{
		Vector2ui coords;
		Color color;
	};

	enum class ColorFormat
	{
		R8G8B8A8_SRGB
	};

	int ColorFormatToChannels(ColorFormat colorFormat);

	ZT_REFLECT_CLASS(NO_CONSTRUCTORS, NO_OPERATORS)
	class ZINET_SOFTWARE_RENDERER_API RenderTarget : public core::Object
	{
	protected:

		inline static core::ConsoleLogger Logger = core::ConsoleLogger::Create("RenderTarget");

	public:

		RenderTarget() = default;
		RenderTarget(const RenderTarget& other) { *this = other; };
		RenderTarget(RenderTarget&& other) = default;

		~RenderTarget() noexcept;

		RenderTarget& operator = (const RenderTarget& other);
		RenderTarget& operator = (RenderTarget&& other) = default;

		bool createEmpty(const Vector2ui& newSize, const ColorFormat newColorFormat);

		bool fill(const Color& color);

		Color getPixelColor(size_t index) const;

		Color getPixelColor(const Vector2ui& pixelCoords) const;

		void clear();

		bool saveToFilePNG(const std::filesystem::path& path) const;

		bool loadFromFilePNG(const std::filesystem::path& path);

		bool writePixelColor(size_t pixelIndex, const Color& color);

		void writePixel(const Pixel& pixel);

		void writePixels(const auto& pixels);

		size_t normalizedCoordsToPixelIndex(const Vector2f& normalized) const;

		Vector2ui normalizedCoordsToPixelCoords(const Vector2f& normalized) const;

		size_t pixelCoordsToPixelIndex(const Vector2ui& pixelCoords) const;

		size_t getBytes() const { return channels * resolution.x * resolution.y; }

		size_t getPixelsCount() const { return resolution.x * resolution.y; }

		stbi_uc* get() { return buffer; }

	protected:

		ZT_REFLECT_MEMBER(ReadOnly)
		Vector2i resolution = { 0u, 0u };

		ZT_REFLECT_MEMBER(ReadOnly)
		ColorFormat colorFormat;

		ZT_REFLECT_MEMBER(ReadOnly)
		int channels = 0u;

		stbi_uc* buffer = nullptr;

	public:
/*GENERATED_CODE_START*/
		static_assert(IsObjectClassInherited); // Class using ZT_REFLECT_CLASS should inherit public from Object class
		const inline static bool RegisterClassResult = RegisterClass<RenderTarget>();
		std::unique_ptr<ObjectBase> createCopy() const override { std::unique_ptr<ObjectBase> result = createCopyInternal<RenderTarget>(); *result = *this; return result; }
		
		class ClassInfo : public zt::core::ClassInfoBase
		{
		public:
		
			std::string_view getClassName() const override { return "RenderTarget"; }
			constexpr static auto GetParentsClassInfo() { return std::vector{core::Object::ClassInfo{}}; }
		};
		const zt::core::ClassInfoBase* getClassInfo() const override { static ClassInfo classInfo; return &classInfo; }
		
		
		const decltype(resolution)& getResolution() const { return resolution; }
		
		const decltype(colorFormat)& getColorFormat() const { return colorFormat; }
		
		const decltype(channels)& getChannels() const { return channels; }
		
/*GENERATED_CODE_END*/

	};

	void RenderTarget::writePixels(const auto& pixels)
	{
#if ZINET_TIME_TRACE
		core::Clock clock;
		clock.start();
#endif

		for (const auto& pixel : pixels)
		{
			const size_t pixelIndex = pixelCoordsToPixelIndex(pixel.coords);
			writePixelColor(pixelIndex, pixel.color);
		}

#if ZINET_TIME_TRACE
		const auto elapsedTime = clock.getElapsedTime().getAsMilliseconds();
		Logger->info("Write pixels count: {} took: {} milliseconds", pixels.size(), elapsedTime);
#endif
	}
}