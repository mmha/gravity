#pragma once

//#include "GravityConfig.h"
#include <lmi/lmi.h>
#include <vector>
#include <experimental/optional>

namespace ge::Core
{
	class Image
	{
		public:
		enum class Channels : uint32_t
		{
			ONE = 1,
			TWO,
			THREE,
			FOUR
		};

		enum class PixelFormat
		{
			UNSIGNED_BYTE,
			FLOAT,
			COUNT
		};

		Image(lmi::vec2ui dimensions, Channels channels, PixelFormat format, void *pixels = nullptr);

		const void *operator()(unsigned int row, unsigned int col) const;
		void *operator()(unsigned int row, unsigned int col);

		const void *operator()(lmi::vec2ui coord) const;
		void *operator()(lmi::vec2ui coord);

		lmi::vec2ui size() const;
		PixelFormat format() const;
		Channels channels() const;
		const void *data() const;

		private:
		lmi::vec2ui dimensions_;
		Channels channels_;
		PixelFormat format_;
		size_t pixelSize_;
		std::vector<char> data_;
	};

	inline size_t pixelSize(Image::PixelFormat p)
	{
		using ElementType = std::underlying_type_t<Image::PixelFormat>;
		const size_t sizes[]{1, 4};
		static_assert(sizeof(sizes) / sizeof(size_t) == (ElementType)Image::PixelFormat::COUNT,
					  "Please update pixelSize after adding a new pixel format");

		return sizes[(ElementType)p];
	}

#ifdef GE_PNG_SUPPORT
	 enum class LoadAlpha
	 {
		 NO, YES
	 };

	std::experimental::optional<Image> loadPNG(const char *filename, LoadAlpha value) noexcept;

	inline std::experimental::optional<Image> loadPNG(const std::string &filename, LoadAlpha value) noexcept
	{
		return loadPNG(filename.c_str(), value);
	}
#endif
}
