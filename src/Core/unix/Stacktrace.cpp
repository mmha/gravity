#include "../Stacktrace.h"
#include <cxxabi.h>
#include <dlfcn.h>
#include <execinfo.h>
#include <memory>
#include <type_traits>

template<typename T>
static auto mallocPtr(T *p)
{
	return std::unique_ptr<T, decltype(std::free) *>(p, &std::free);
}

namespace ge::Core
{

auto stackTrace() -> std::vector<std::string>
{
	auto trace = std::array<void *, 256>{};
	auto frameCount = backtrace(trace.data(), trace.size());
	auto symbols = mallocPtr(backtrace_symbols(trace.data(), frameCount));
	auto result = std::vector<std::string>(frameCount);

	for(int i = 0; i < frameCount; ++i)
	{
		Dl_info info;
		if(dladdr(trace[i], &info) && info.dli_sname)
		{
			auto status = int{};
			auto demangledSym = mallocPtr(abi::__cxa_demangle(info.dli_sname, nullptr, nullptr, &status));
			if(status == 0)
			{
				result[i] = demangledSym.get();
				continue;
			}
		}

		result[i] = symbols.get()[i];
	}
	return result;
}

void printStackTrace(std::ostream &out)
{
	auto trace = stackTrace();
	for (auto &frame : trace)
	{
		out << frame << std::endl;
	}
}

}