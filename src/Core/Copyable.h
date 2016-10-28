#pragma once

#include <type_traits>

namespace ge::Core
{
	enum class Copyable
	{
		NO, YES
	};

	struct CopyableTag {};
	struct NoncopyableTag
	{
		IsNoncopyable(const IsNoncopyable &) = delete;
		IsNoncopyable(IsNoncopyable &&) noexcept = default;
		IsNoncopyable &operator=(IsNoncopyable &&) noexcept = default;
	};

	template<bool expression>
	using CopyableIf = std::conditional_t<expression, CopyableTag, NoncopyableTag>;

	template<Copyable C>
	using ConditionallyCopyable = CopyableIf<C == Copyable::YES>;
}