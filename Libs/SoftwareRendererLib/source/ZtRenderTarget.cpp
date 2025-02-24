#include "Zinet/SoftwareRenderer/ZtRenderTarget.hpp"

#include "Zinet/Core/ZtFile.hpp"

namespace zt::software_renderer
{
	inline int ColorFormatToChannels(ColorFormat colorFormat)
	{
		if (colorFormat == ColorFormat::R8G8B8A8_SRGB)
		{
			return 4u;
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

	bool RenderTarget::createEmpty(const Vector2ui& newResolution, const ColorFormat newColorFormat)
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

		if (newResolution.x <= 0u || newResolution.y <= 0u)
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

		const size_t bytes = getBytes();
		for (size_t index = 0u; index < bytes; index += channels)
		{
			std::memcpy(&buffer[index], &color, channels);
		}

#if ZINET_TIME_TRACE
		const auto elapsedTime = clock.getElapsedTime().getAsMilliseconds();
		Logger->info("Fill render target took: {} milliseconds", elapsedTime);
#endif

		return true;
	}

	Color RenderTarget::getPixelColor(size_t index) const
	{
		const size_t pixelsCount = getPixelsCount();
		if (index >= pixelsCount)
		{
			Logger->error("Pixel index is out of the render target bounds Resolution: {}, {} PixelIndex: {}", resolution.x, resolution.y, index);
			return {};
		}
		index *= channels;

		Color result;
		std::memcpy(&result, &buffer[index], channels);

		return result;
	}

	Color RenderTarget::getPixelColor(const Vector2ui& pixelCoords) const
	{
		const Color result = getPixelColor(pixelCoordsToPixelIndex(pixelCoords));
		return result;
	}

	void RenderTarget::clear()
	{
		if (buffer)
		{
			stbi_image_free(buffer);
			buffer = nullptr;
			resolution = Vector2ui{};
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

	bool RenderTarget::writePixelColor(size_t pixelIndex, const Color& color)
	{
		const size_t pixelsCount = getPixelsCount();
		if (pixelIndex >= pixelsCount)
		{
			Logger->error("Pixel index is out of the render target bounds Resolution: {}, {} PixelIndex: {}", resolution.x, resolution.y, pixelIndex);
			return false;
		}

		pixelIndex *= channels;

		std::memcpy(&buffer[pixelIndex], &color, channels);

		return true;
	}

	void RenderTarget::writePixel(const Pixel& pixel)
	{
		const size_t pixelIndex = pixelCoordsToPixelIndex(pixel.coords);
		writePixelColor(pixelIndex, pixel.color);
	}

	size_t RenderTarget::normalizedCoordsToPixelIndex(const Vector2f& normalized) const
	{
		const Vector2ui pixelCoords = normalizedCoordsToPixelCoords(normalized);
		const size_t pixelIndex = pixelCoordsToPixelIndex(pixelCoords);
		return pixelIndex;
	}

	Vector2ui RenderTarget::normalizedCoordsToPixelCoords(const Vector2f& normalized) const
	{
		const Vector2ui pixelCoords = { normalized.x * resolution.x, normalized.y * resolution.y };
		return pixelCoords;
	}

	size_t RenderTarget::pixelCoordsToPixelIndex(const Vector2ui& pixelCoords) const
	{
		const size_t pixelIndex = pixelCoords.y * resolution.x + pixelCoords.x;
		return pixelIndex;
	}

}