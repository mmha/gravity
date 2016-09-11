#include "../Stacktrace.h"
#include <Dbghelp.h>
#include <windows.h>
#include <array>
#include <memory>

namespace ge::Core
{

auto stackTrace() -> std::vector<std::string>
{
	static auto thisProcess = GetCurrentProcess();
	static auto executeOnce = SymInitialize(thisProcess, nullptr, true);
	(void)executeOnce;

	std::array<void *, 256> backtrace;
	auto frameCount = CaptureStackBackTrace(1, backtrace.size(), backtrace.data(), nullptr);
	
	auto buf = std::make_unique<char[]>(sizeof(SYMBOL_INFO) + 512);
	auto symbolInfo = reinterpret_cast<SYMBOL_INFO *>(buf.get());
	symbolInfo->MaxNameLen = 511;
	symbolInfo->SizeOfStruct = sizeof(SYMBOL_INFO);

	auto result = std::vector<std::string>(frameCount);
	for (int i = 0; i < frameCount; ++i)
	{
		SymFromAddr(thisProcess, (DWORD64) (backtrace[i]), nullptr, symbolInfo);
		result.emplace_back(std::to_string(i) + ": " + symbolInfo->Name);
	}
	return result;
}

void printStackTrace(std::ostream &out)
{
	auto trace = stackTrace();
	for (auto &frame : trace)
	{
		out << frame << '\n';
	}
}

}