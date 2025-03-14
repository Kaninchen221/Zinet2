#pragma once

#include "Zinet/SoftwareRenderer/ZtSoftwareRendererConfig.hpp"
#include "Zinet/SoftwareRenderer/ZtTypes.hpp"

#include "Zinet/Core/ZtLogger.hpp"
#include "Zinet/Core/Reflection/ZtReflection.hpp"
#include "Zinet/Core/ZtClock.hpp"
#include "Zinet/Core/ZtCoreConfig.hpp"

#include "Zinet/Math/ZtVecTypes.hpp"

#include <filesystem>
#include <memory>

namespace zt::software_renderer
{
	int ColorFormatToChannels(ColorFormat colorFormat);

	// Some methods lack validation due to optimization
	ZT_REFLECT_CLASS(NO_CONSTRUCTORS, NO_OPERATORS)
	class ZINET_SOFTWARE_RENDERER_API RenderTarget : public core::Object
	{
	protected:

		inline static core::ConsoleLogger Logger = core::ConsoleLogger::Create("RenderTarget");

	public:

		RenderTarget() = default;
		RenderTarget(const RenderTarget& other) { *this = other; };
		RenderTarget(RenderTarget&& other) = default;

		RenderTarget& operator = (const RenderTarget& other) = default;
		RenderTarget& operator = (RenderTarget&& other) = default;
		
		RenderTarget copy() const;

		// TODO: Change name to "create"
		bool createEmpty(const Vector2i& newSize, const ColorFormat newColorFormat);

		bool fill(const Color& color);

		Color getPixelColor(std::int32_t index) const;

		Color* getPixelColorAddr(std::int32_t index) const;

		Color getPixelColor(const Vector2i& pixelCoords) const;

		Color* getPixelColorAddr(const Vector2i& pixelCoords);

		void clear();

		bool saveToFilePNG(const std::filesystem::path& path) const;

		bool loadFromFilePNG(const std::filesystem::path& path);

		bool writePixelColor(std::int32_t pixelIndex, const Color& color);

		void writePixel(const Pixel& pixel);

		void writePixels(const auto& pixels);

		std::int32_t normalizedCoordsToPixelIndex(const Vector2f& normalized) const;

		Vector2i normalizedCoordsToPixelCoords(const Vector2f& normalized) const;

		std::int32_t pixelCoordsToPixelIndex(const Vector2i& pixelCoords) const;

		std::int32_t getBytes() const { return channels * resolution.x * resolution.y; }

		std::int32_t getPixelsCount() const { return resolution.x * resolution.y; }

		stbi_uc* get() { return buffer.get(); }

		bool isValid() const { return buffer.operator bool(); }

		bool isPixelIndexValid(std::int32_t pixelIndex) const;

		bool areCoordsValid(const Vector2i& coords) const;

	protected:

		ZT_REFLECT_MEMBER(ReadOnly)
		Vector2i resolution = { 0, 0 };

		ZT_REFLECT_MEMBER(ReadOnly)
		ColorFormat colorFormat;

		ZT_REFLECT_MEMBER(ReadOnly)
		std::int32_t channels = 0;

		std::shared_ptr<stbi_uc> buffer;

	public:
/*GENERATED_CODE_START*/
		static_assert(IsObjectClassInherited); // Class using ZT_REFLECT_CLASS should inherit public from Object class
		const inline static bool RegisterClassResult = RegisterClass<RenderTarget>();
		std::unique_ptr<ObjectBase> createCopy() const override { std::unique_ptr<ObjectBase> result = createCopyInternal<RenderTarget>(); *result = *this; return result; }
		
		~RenderTarget() noexcept = default;
		
		class ClassInfo : public zt::core::ClassInfoBase
		{
		public:
		
			std::string_view getClassName() const override { return "RenderTarget"; }
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
			const std::int32_t pixelIndex = pixelCoordsToPixelIndex(pixel.coords);
			writePixelColor(pixelIndex, pixel.color);
		}

#if ZINET_TIME_TRACE
		const auto elapsedTime = clock.getElapsedTime().getAsMilliseconds();
		Logger->info("Write pixels count: {} took: {} milliseconds", pixels.size(), elapsedTime);
#endif
	}
}