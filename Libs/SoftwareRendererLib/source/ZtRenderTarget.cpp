#include "Zinet/SoftwareRenderer/ZtRenderTarget.hpp"

#include "Zinet/Core/ZtFile.hpp"

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
			Logger->error("Invalid ColorFormat: {}", colorFormat);
			return -1;
		}
	}

	RenderTarget& RenderTarget::operator=(const RenderTarget& other)
	{
		createEmpty(other.resolution, other.colorFormat);

		std::memcpy(buffer, other.buffer, getBytes());

		return *this;
	}

	RenderTarget::~RenderTarget() noexcept
	{
		clear();
	}

	bool RenderTarget::createEmpty(const Vector2i& newResolution, const ColorFormat newColorFormat)
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
		buffer = reinterpret_cast<stbi_uc*>(std::malloc(bytes));

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
			std::memcpy(&buffer[index], &color, channels);
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
		if (!core::Ensure(isPixelIndexValid(index)))
			return Color{};
#endif
		index *= channels;

		Color result;
		std::memcpy(&result, &buffer[index], channels);

		return result;
	}

	Color* RenderTarget::getPixelColorAddr(std::int32_t index) const
	{
		return reinterpret_cast<Color*>(&buffer[index]);
	}

	Color RenderTarget::getPixelColor(const Vector2i& pixelCoords) const
	{
		const Color result = getPixelColor(pixelCoordsToPixelIndex(pixelCoords));
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
			stbi_image_free(buffer);
			buffer = nullptr;
			resolution = Vector2i{};
			channels = 0u;
		}
	}

	bool RenderTarget::saveToFilePNG(const std::filesystem::path& path) const
	{
		const std::string pathString = path.string();
		auto result = stbi_write_png(pathString.c_str(), resolution.x, resolution.y, channels, buffer, 0);
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
		buffer = stbi_load(pathString.c_str(), &resolution.x, &resolution.y, &channels, 4);

		return true;
	}

	bool RenderTarget::writePixelColor(std::int32_t pixelIndex, const Color& color)
	{
#if ZINET_DEBUG
		if (!core::Ensure(isPixelIndexValid(pixelIndex)))
			return false;
#endif
		pixelIndex *= channels;

		std::memcpy(&buffer[pixelIndex], &color, channels);

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
		const std::int32_t pixelIndex = (pixelCoords.y * resolution.x + pixelCoords.x) * channels;
		return pixelIndex;
	}

	bool RenderTarget::isPixelIndexValid(std::int32_t pixelIndex) const
	{
		return pixelIndex >= 0 && pixelIndex < getPixelsCount();
	}

	bool RenderTarget::areCoordsValid(const Vector2i& coords) const
	{
		if (std::cmp_less_equal(resolution.x, coords.x) || std::cmp_less_equal(resolution.y, coords.y))
			return false;

		return true;
	}

}