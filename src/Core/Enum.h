#pragma once
#include <bibi/Core.h>

#define GE_DECLARE_CONTIGUOUS_ENUM_UTILS(E) \
	friend bool operator<(E lhs, E rhs) \
	{ \
		return static_cast<std::underlying_type_t<E>>(lhs) < static_cast<std::underlying_type_t<E>>(rhs); \
	} \
	friend bool operator<=(E lhs, E rhs) \
	{ \
		return static_cast<std::underlying_type_t<E>>(lhs) <= static_cast<std::underlying_type_t<E>>(rhs); \
	} \
	friend bool operator>(E lhs, E rhs) \
	{ \
		return static_cast<std::underlying_type_t<E>>(lhs) > static_cast<std::underlying_type_t<E>>(rhs); \
	} \
	friend bool operator>=(E lhs, E rhs) \
	{ \
		return static_cast<std::underlying_type_t<E>>(lhs) >= static_cast<std::underlying_type_t<E>>(rhs); \
	}

#define GE_DECLRARE_ENUM_STD_HASHER(E) \
	namespace std \
	{ \
		template<> \
		struct hash<E> \
		{ \
			auto operator()(E e) -> size_t \
			{ \
				return static_cast<size_t>(e); \
			} \
		}; \
	}

namespace ge::Core
{
	template<typename T, bibi::Enum E>
	class EnumArray : public std::array<T, static_cast<size_t>(E::COUNT)>
	{
	public:
		using underlying_type = E;
		using base = std::array<T, static_cast<size_t>(E::COUNT)>;

		template<typename... Args>
		constexpr EnumArray(Args &&... args)
		: base{std::forward<Args>(args)...}
		{
		}

		constexpr auto operator[](E val) -> T &
		{
			return base::operator[](static_cast<std::underlying_type_t<E>>(val));
		}

		constexpr auto operator[](E val) const -> const T &
		{
			return base::operator[](static_cast<std::underlying_type_t<E>>(val));
		}
	};
}