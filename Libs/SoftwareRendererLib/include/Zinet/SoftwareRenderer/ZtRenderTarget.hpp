#pragma once

#include "Zinet/SoftwareRenderer/ZtSoftwareRendererConfig.hpp"

#include "Zinet/Core/ZtLogger.hpp"
#include "Zinet/Core/Reflection/ZtReflection.hpp"

#include "Zinet/Math/ZtVecTypes.hpp"

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

	ZT_REFLECT_CLASS(NO_CONSTRUCTORS)
	class ZINET_SOFTWARE_RENDERER_API RenderTarget : public core::Object
	{
	protected:

		inline static core::ConsoleLogger Logger = core::ConsoleLogger::Create("RenderTarget");

	public:

		RenderTarget() = default;
		RenderTarget(const RenderTarget& other) = default;
		RenderTarget(RenderTarget&& other) = default;

		~RenderTarget() noexcept;

		bool create(const Vector2ui& newSize, const ColorFormat newColorFormat);

		bool fill(const Color& color);

		Color getPixelColor(size_t index) const;

		void clear();

		bool saveToFilePNG(const std::filesystem::path& path) const;

		bool loadFromFilePNG(const std::filesystem::path& path);

		bool writePixelColor(size_t pixelIndex, const Color& color);

		void writePixels(const auto& pixels);

		size_t normalizedCoordsToPixelIndex(const Vector2f& normalized) const;

		Vector2ui normalizedCoordsToPixelCoords(const Vector2f& normalized) const;

		size_t pixelCoordsToPixelIndex(const Vector2ui& pixelCoords) const;

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
		
		RenderTarget& operator = (const RenderTarget& other) = default;
		RenderTarget& operator = (RenderTarget&& other) = default;
		
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
		for (const auto& pixel : pixels)
		{
			const size_t pixelIndex = pixelCoordsToPixelIndex(pixel.coords);
			writePixelColor(pixelIndex, pixel.color);
		}
	}
}