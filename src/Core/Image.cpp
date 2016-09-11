#include "Image.h"
#include <algorithm>

#ifdef GE_PNG_SUPPORT
#include "MappedFile.h"
#include <memory>
#include <png.h>
#endif

namespace ge::Core
{

Image::Image(lmi::vec2ui dimensions, Channels channels, PixelFormat format, void *pixels)
	: dimensions_(dimensions)
	, channels_(channels)
	, format_(format)
	, pixelSize_(pixelSize(format))
	, data_((uint32_t)channels * dimensions.x * dimensions.y * pixelSize(format))
{
	if(pixels)
	{
		auto pixelPtr = static_cast<char *>(pixels);
		std::copy(pixelPtr, pixelPtr + data_.size(), data_.begin());
	}
}

const void *Image::operator()(unsigned int row, unsigned int col) const
{
	return &data_[row * dimensions_.x * pixelSize_ + col * pixelSize_];
}

void *Image::operator()(unsigned int row, unsigned int col)
{
	return &data_[row * dimensions_.x * pixelSize_ + col * pixelSize_];
}

const void *Image::operator()(lmi::vec2ui coord) const
{
	return &data_[coord.y * dimensions_.x * pixelSize_ + coord.x * pixelSize_];
}

void *Image::operator()(lmi::vec2ui coord)
{
	return &data_[coord.y * dimensions_.x * pixelSize_ + coord.x * pixelSize_];
}

lmi::vec2ui Image::size() const
{
	return dimensions_;
}

Image::PixelFormat Image::format() const
{
	return format_;
}

Image::Channels Image::channels() const
{
	return channels_;
}

const void *Image::data() const
{
	return &data_[0];
}

#ifdef GE_PNG_SUPPORT
std::experimental::optional<Image> loadPNG(const char *filename, LoadAlpha value) noexcept
{
	auto maybeFile = MappedFile::create(filename);
	if (!maybeFile)
	{
		return std::nullopt;
	}
	auto &file = *maybeFile;

	const size_t headerSize = 8;
	auto is_png = !png_sig_cmp((png_const_bytep)file.cdata(), 0, headerSize);
	if(!is_png)
	{
		return std::experimental::nullopt;
	}

	auto pngStruct = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
	if(!pngStruct)
	{
		return std::experimental::nullopt;
	}

	auto pngInfo = png_create_info_struct(pngStruct);
	if(!pngInfo)
	{
		png_destroy_read_struct(&pngStruct, nullptr, nullptr);
		return std::experimental::nullopt;
	}

	auto pngEndInfo = png_create_info_struct(pngStruct);
	if(!pngEndInfo)
	{
		png_destroy_read_struct(&pngStruct, &pngInfo, nullptr);
		return std::experimental::nullopt;
	}

	if(setjmp(png_jmpbuf(pngStruct)))
	{
		png_destroy_read_struct(&pngStruct, &pngInfo, &pngEndInfo);
		return std::experimental::nullopt;
	}

	// FIXME range check to avoid end of file errors
	png_set_read_fn(pngStruct, file.data(), [](png_structp png_ptr, png_bytep data, png_size_t length) mutable {
		auto fileBegin = reinterpret_cast<char *>(png_get_io_ptr(png_ptr));
		static size_t offset = 0;
		std::copy(fileBegin + offset, fileBegin + offset + length, data);
		offset += length;
	});
	auto transforms = PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | (value == LoadAlpha::NO ? PNG_TRANSFORM_STRIP_ALPHA : 0);
	png_read_png(pngStruct, pngInfo, transforms, nullptr);
	auto rowPointers = png_get_rows(pngStruct, pngInfo);

	lmi::vec2ui imageSize{png_get_image_width(pngStruct, pngInfo), png_get_image_height(pngStruct, pngInfo)};
	uint32_t channels = png_get_channels(pngStruct, pngInfo);
	if(channels > 4)
	{
		png_destroy_read_struct(&pngStruct, &pngInfo, &pngEndInfo);
		return std::nullopt;
	}

	auto uselessBuffer = std::make_unique<uint8_t[]>(imageSize.x * imageSize.y * channels);
	for(uint32_t i = 0; i < imageSize.y; ++i)
	{
		auto begin   = rowPointers[imageSize.y - i - 1];
		auto rowSize = imageSize.x * channels;
		std::copy(begin, begin + rowSize, uselessBuffer.get() + i * rowSize);
	};

	png_destroy_read_struct(&pngStruct, &pngInfo, &pngEndInfo);
	return Image{imageSize, (Image::Channels)channels, Image::PixelFormat::UNSIGNED_BYTE, uselessBuffer.get()};
}
#endif

}
