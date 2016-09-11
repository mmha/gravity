#pragma once

#include "Sigslot.h"

namespace ge::Core
{
	class AttributeBase
	{
		virtual ~AttributeBase() = default;
	};
	
	template<typename T, LockPolicy lockPolicy = multi_threaded_local>
	class Attribute final : public AttributeBase
	{
	private:
		T value_;
	public:
		Signal<lockPolicy, Attribute<T> &> changed;
		
		template<typename... Args>
		explicit Attribute(Args... args)
		: value_(std::forward<Args>(args)...)
		{			
		}
		
		operator const T&() const
		{
			return value_;
		}
		
		auto value() -> T &
		{
			return value_;
		}

		auto operator->() const -> T &
		{
			return value_;
		}

		auto operator->() -> T &
		{
			return value_;
		}
		
		template<typename U>
		Attribute &operator=(U &&newValue) 
		{
			value_ = std::forward<U>(newValue);
			changed(*this);
			return *this;
		}
	};

}
