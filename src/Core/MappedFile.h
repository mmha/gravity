#pragma once

#include <string>

#ifdef _WIN32
#include <windows.h>
#endif

namespace ge::Core
{
	class MappedFile
	{
		public:
		enum Access
		{
			READ	  = (1 << 0),
			WRITE	  = (1 << 1),
			EXCLUSIVE = (1 << 2),
		};

		enum UsageHint
		{
			NORMAL,
			SEQUENTIAL,
			RANDOM_ACCESS
		};

		explicit MappedFile(const std::string &filename, Access access = READ, UsageHint usageHint = NORMAL);
		explicit MappedFile(const char *filename, Access access = READ, UsageHint usageHint = NORMAL);
		MappedFile(const MappedFile &) = delete;
		MappedFile(MappedFile &&other) noexcept;
		MappedFile &operator=(MappedFile &&other) noexcept;
		~MappedFile();

		auto operator[](size_t i) const -> char;
		auto operator[](size_t i) -> char &;

		auto data() const -> char *;
		auto cdata() const -> const char *;

		auto size() const -> size_t;

		auto begin() -> char *;
		auto end() -> char *;
		auto begin() const -> const char *;
		auto end() const -> const char *;

		private:
		char *file_;
		size_t fileSize_;

#ifdef __unix__
		int fd_;
#endif
#ifdef _WIN32
		HANDLE fileHandle_;
		HANDLE fileMappingObject_;
#endif
	};
}
