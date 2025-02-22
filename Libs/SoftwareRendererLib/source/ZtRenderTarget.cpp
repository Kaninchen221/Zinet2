#include "Zinet/SoftwareRenderer/ZtRenderTarget.hpp"

#include "Zinet/Core/ZtFile.hpp"

namespace zt::software_renderer
{
	RenderTarget::~RenderTarget() noexcept
	{
		clear();
	}

	bool RenderTarget::create(const Vector2ui& newResolution, const ColorFormat newColorFormat)
	{
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

		if (newColorFormat == ColorFormat::R8G8B8A8_SRGB)
		{
			channels = 4u;
		}
		else
		{
			Logger->error("Invalid ColorFormat: {}", newColorFormat);
			return false;
		}

		resolution = newResolution;
		colorFormat = newColorFormat;

		const auto bytes = channels * resolution.x * resolution.y;
		buffer = (stbi_uc*)std::malloc(bytes);

		return true;
	}

	bool RenderTarget::fill(const Color& color)
	{
		// Naive implementation it's good enough for now

		if (channels != color.length())
		{
			Logger->error("color components count must be equal to channels");
			return false;
		}

		const size_t pixelsCount = resolution.x * resolution.y * channels;
		for (size_t index = 0u; index < pixelsCount; index += channels)
		{
			buffer[index] = color.r;
			buffer[index + 1] = color.g;
			buffer[index + 2] = color.b;
			buffer[index + 3] = color.a;
		}

		return true;
	}

	Color RenderTarget::getPixelColor(size_t index) const
	{
		const size_t pixelsCount = resolution.x * resolution.y;
		if (index >= pixelsCount)
		{
			Logger->error("Pixel index is out of the render target bounds Resolution: {}, {} PixelIndex: {}", resolution.x, resolution.y, index);
			return {};
		}
		index *= channels;

		Color result;
		result.r = buffer[index];
		result.g = buffer[index + 1];
		result.b = buffer[index + 2];
		result.a = buffer[index + 3];

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
		const size_t pixelsCount = resolution.x * resolution.y;
		if (pixelIndex >= pixelsCount)
		{
			Logger->error("Pixel index is out of the render target bounds Resolution: {}, {} PixelIndex: {}", resolution.x, resolution.y, pixelIndex);
			return false;
		}

		pixelIndex *= channels;

		buffer[pixelIndex] = color.r;
		buffer[pixelIndex + 1] = color.g;
		buffer[pixelIndex + 2] = color.b;
		buffer[pixelIndex + 3] = color.a;

		return true;
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