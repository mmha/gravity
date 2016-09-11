#pragma once

#include <vector>
#include <string>
#include <iostream>

/**
 *  Warning: These functions require memory allocation, do not use them on std::bad_alloc
 */

namespace ge::Core
{
	auto stackTrace() -> std::vector<std::string>;
	void printStackTrace(std::ostream &out = std::cout);

	template<typename Exception>
	class StackTraced : public Exception
	{
		public:
		template<typename... Args>
		StackTraced(Args&&... args)
		: Exception{std::forward<Args>(args)...}
		, stackTrace_{stackTrace()}
		{

		}

		auto what() const noexcept -> const char * override
		{
			return Exception::what();
		}

		auto stackTrace() const noexcept -> const std::vector<std::string> &
		{
			return stackTrace_;
		}

		void printStackTrace(std::ostream &out = std::cout) const
		{
			::ge::Core::printStackTrace(out);
		}
	private:
		std::vector<std::string> stackTrace_;
	};
}
