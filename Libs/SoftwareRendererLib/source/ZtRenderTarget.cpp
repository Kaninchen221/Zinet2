#include "Zinet/SoftwareRenderer/ZtRenderTarget.hpp"

#include "Zinet/Core/ZtFile.hpp"
#include "Zinet/Core/ZtTypeTraits.hpp"

namespace zt::software_renderer
{
	inline std::int32_t ColorFormatToChannels(ColorFormat colorFormat)
	{
		if (colorFormat == ColorFormat::R8G8B8A8_SRGB)
		{
			return 4;
		}
		else
		{
			auto Logger = core::ConsoleLogger::Create("ColorFormatToChannels");
			Logger->error("Invalid ColorFormat: {}", static_cast<int>(colorFormat));
			return -1;
		}
	}

	RenderTarget RenderTarget::copy() const
	{
		RenderTarget result;
		result.create(resolution, colorFormat);

		std::memcpy(result.buffer.get(), buffer.get(), getBytes());

		return *this;
	}

	bool RenderTarget::create(const Vector2i& newResolution, const ColorFormat newColorFormat)
	{
#if ZINET_TIME_TRACE
		core::Clock clock;
		clock.start();
#endif

		if (buffer)
		{
			Logger->error("RenderTarget already has valid buffer inside destroy the render target first");
			return false;
		}

		if (newResolution.x <= 0 || newResolution.y <= 0)
		{
			Logger->error("Size x and y must be greater than 0");
			return false;
		}

		channels = ColorFormatToChannels(newColorFormat);
		if (channels == -1)
			return false;

		resolution = newResolution;
		colorFormat = newColorFormat;

		const auto bytes = getBytes();
		buffer = std::shared_ptr<stbi_uc>(
			reinterpret_cast<stbi_uc*>(std::malloc(bytes)), 
			core::LambdaFree
		);

#if ZINET_TIME_TRACE
		const auto elapsedTime = clock.getElapsedTime().getAsMilliseconds();
		Logger->info("CreateEmpty render target took: {} milliseconds", elapsedTime);
#endif

		return true;
	}

	bool RenderTarget::fill(const Color& color)
	{
#if ZINET_TIME_TRACE
		core::Clock clock;
		clock.start();
#endif

		if (channels != color.length())
		{
			Logger->error("Color components count must be equal to channels");
			return false;
		}

		const std::int32_t bytes = getBytes();
		for (std::int32_t index = 0; index < bytes; index += channels)
		{
			std::memcpy(&buffer.get()[index], &color, channels);
		}

#if ZINET_TIME_TRACE
		const auto elapsedTime = clock.getElapsedTime().getAsMilliseconds();
		Logger->info("Fill render target took: {} milliseconds", elapsedTime);
#endif

		return true;
	}

	Color RenderTarget::getPixelColor(std::int32_t index) const
	{
#if ZINET_DEBUG
		if (!isPixelIndexValid(index))
		{
			Logger->error("Tried to access pixel color at invalid index: {}", index);
			return Color{};
		}
#endif
		index *= channels;

		Color result;
		std::memcpy(&result, &buffer.get()[index], channels);

		return result;
	}

	Color* RenderTarget::getPixelColorAddr(std::int32_t index) const
	{
		index *= channels;
		return reinterpret_cast<Color*>(&buffer.get()[index]);
	}

	Color RenderTarget::getPixelColor(const Vector2i& pixelCoords) const
	{
		const std::int32_t pixelIndex = pixelCoordsToPixelIndex(pixelCoords);
		const Color result = getPixelColor(pixelIndex);
		return result;
	}

	Color* RenderTarget::getPixelColorAddr(const Vector2i& pixelCoords)
	{
		const std::int32_t pixelIndex = pixelCoordsToPixelIndex(pixelCoords);
		return getPixelColorAddr(pixelIndex);
	}

	void RenderTarget::clear()
	{
		if (buffer)
		{
			buffer = std::shared_ptr<stbi_uc>();
			resolution = Vector2i{};
			channels = 0;
		}
	}

	bool RenderTarget::saveToFilePNG(const std::filesystem::path& path) const
	{
		const std::string pathString = path.string();
		auto result = stbi_write_png(pathString.c_str(), resolution.x, resolution.y, channels, buffer.get(), 0);
		return result == 1;
	}

	bool RenderTarget::loadFromFilePNG(const std::filesystem::path& path)
	{
		if (buffer)
		{
			clear();
		}

		if (!std::filesystem::exists(path))
		{
			Logger->error("Can't load from file, file doesn't exist: {}", path.string());
			return false;
		};

		const std::string pathString = path.string();
		buffer = std::shared_ptr<stbi_uc>(
			stbi_load(pathString.c_str(), &resolution.x, &resolution.y, &channels, Color{}.length()),
			core::LambdaFree
		);

		if (channels != Color{}.length())
		{
			Logger->error("Loaded texture but channels count is invalid: {}, Required: {}", channels, Color{}.length());
			return false;
		}

		return true;
	}

	bool RenderTarget::writePixelColor(std::int32_t pixelIndex, const Color& color)
	{
#if ZINET_DEBUG
		if (!core::Ensure(isPixelIndexValid(pixelIndex)))
			return false;
#endif
		pixelIndex *= channels;

		std::memcpy(&buffer.get()[pixelIndex], &color, channels);

		return true;
	}

	void RenderTarget::writePixel(const Pixel& pixel)
	{
		const std::int32_t pixelIndex = pixelCoordsToPixelIndex(pixel.coords);
		writePixelColor(pixelIndex, pixel.color);
	}

	std::int32_t RenderTarget::normalizedCoordsToPixelIndex(const Vector2f& normalized) const
	{
		const Vector2i pixelCoords = normalizedCoordsToPixelCoords(normalized);
		const std::int32_t pixelIndex = pixelCoordsToPixelIndex(pixelCoords);
		return pixelIndex;
	}

	Vector2i RenderTarget::normalizedCoordsToPixelCoords(const Vector2f& normalized) const
	{
		const Vector2i pixelCoords = 
		{ 
			normalized.x * resolution.x,
			normalized.y * resolution.y
		};

		return pixelCoords;
	}

	std::int32_t RenderTarget::pixelCoordsToPixelIndex(const Vector2i& pixelCoords) const
	{
		const std::int32_t pixelIndex = pixelCoords.y * resolution.x + pixelCoords.x;
		return pixelIndex;
	}

	bool RenderTarget::isPixelIndexValid(std::int32_t pixelIndex) const
	{
		return pixelIndex >= 0 && pixelIndex < getPixelsCount();
	}

	bool RenderTarget::areCoordsValid(const Vector2i& coords) const
	{
		if (resolution.x <= coords.x || resolution.y <= coords.y || coords.x < 0 || coords.y < 0)
			return false;

		return true;
	}

}