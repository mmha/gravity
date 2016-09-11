#pragma once

#include <bibi/Algorithm.h>
#include <bibi/Core.h>
#include <iterator>
#include <limits>

namespace ge::Core
{
	namespace internal
	{
		constexpr char base64LUT[66] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";
	}

	constexpr size_t base64_size(size_t size)
	{
		return 4 * ((size + 2) / 3);
	}

	template<bibi::NullablePointer Pointer>
	constexpr void to_base64(const char *begin, const char *end, bibi::OutputIterator out)
	{
		auto push_back = [out](auto byte) mutable {
			*out = byte;
			++out;
		};

		size_t length = end - begin;

		uint8_t b = 0;
		for (size_t i = 0; i < length; i += 3)
		{
			b = (begin[i] & 0xFC) >> 2;
			push_back(internal::base64LUT[b]);
			b = (begin[i] & 0x03) << 4;
			if (i + 1 < length)
			{
				b |= (begin[i + 1] & 0xF0) >> 4;
				push_back(internal::base64LUT[b]);
				b = (begin[i + 1] & 0x0F) << 2;
				if (i + 2 < length)
				{
					b |= (begin[i + 2] & 0xC0) >> 6;
					push_back(internal::base64LUT[b]);
					b = begin[i + 2] & 0x3F;
					push_back(internal::base64LUT[b]);
				}
				else
				{
					push_back(internal::base64LUT[b]);
					push_back('=');
				}
			}
			else
			{
				push_back(internal::base64LUT[b]);
				push_back('=');
				push_back('=');
			}
		}
	}

	constexpr bool from_base64(const char *begin, const char *end, bibi::OutputIterator out)
	{
		auto push_back = [out](auto byte) mutable {
			*out = byte;
			++out;
		};

		size_t length = end - begin;
		if (length % 4 != 0)
		{
			return false;
		}

		uint8_t b[4];
		for (size_t i = 0; i < length; i += 4)
		{
			for (uint32_t k = 0; k < 4; ++k)
			{
				b[k] = bibi::find(internal::base64LUT, internal::base64LUT + sizeof(internal::base64LUT) - 1,
					begin[i + k]) -
					internal::base64LUT;
			}
			push_back((b[0] << 2) | (b[1] >> 4));
			if (b[2] < 64)
			{
				push_back((b[1] << 4) | (b[2] >> 2));
				if (b[3] < 64)
				{
					push_back((b[2] << 6) | b[3]);
				}
			}
		}

		return true;
	}

	inline std::string operator"" _base64(const char *str, size_t len)
	{
		std::string result;
		result.reserve(base64_size(len));
		to_base64(str, str + len, std::back_inserter(result));
		return result;
	}
}
