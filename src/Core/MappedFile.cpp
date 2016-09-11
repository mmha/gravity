#include "MappedFile.h"

namespace ge::Core
{

MappedFile::MappedFile(const std::string &filename, Access access, UsageHint usageHint)
: MappedFile(filename.c_str(), access, usageHint)
{
}

auto MappedFile::operator[](size_t i) const -> char
{
	return file_[i];
}

auto MappedFile::operator[](size_t i) -> char &
{
	return file_[i];
}

auto MappedFile::data() const -> char *
{
	return file_;
}

auto MappedFile::cdata() const -> const char *
{
	return file_;
}

auto MappedFile::size() const -> size_t
{
	return fileSize_;
}

auto MappedFile::begin() -> char *
{
	return file_;
}

auto MappedFile::end() -> char *
{
	return file_ + fileSize_;
}

auto MappedFile::begin() const -> const char *
{
	return file_;
}

auto MappedFile::end() const -> const char *
{
	return file_ + fileSize_;
}

}
