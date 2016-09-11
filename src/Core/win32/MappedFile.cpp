#include "../MappedFile.h"

namespace ge::Core
{

MappedFile::MappedFile(const char *filename, Access access, UsageHint usageHint)
{
	if(access & WRITE && !(access & READ))
	{
		throw std::invalid_argument("Invalid access bit set");
	}

	DWORD accessMask = 0;
	DWORD shareMask  = 0;
	switch(access & ~EXCLUSIVE)
	{
		case READ:
			accessMask = GENERIC_READ | GENERIC_EXECUTE;
			break;
		case READ | WRITE:
			accessMask = GENERIC_ALL;
			break;
	}
	if(access & EXCLUSIVE)
	{
		shareMask |= FILE_SHARE_READ | FILE_SHARE_WRITE;
	}

	fileHandle_ = CreateFile(filename, accessMask, shareMask, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	if(fileHandle_ == INVALID_HANDLE_VALUE)
	{
		return nullopt;
	}

	// Get File Size
	{
		DWORD high, low;
		low		  = GetFileSize(fileHandle_, &high);
		fileSize_ = (size_t{high} << 32ULL) + low;
	}

	DWORD pageProtectionMask = 0;
	switch(access & ~EXCLUSIVE)
	{
		case READ:
			pageProtectionMask = PAGE_EXECUTE_READ;
			break;
		case READ | WRITE:
			pageProtectionMask = PAGE_EXECUTE_READWRITE;
			break;
	}
	fileMappingObject_ = CreateFileMapping(fileHandle_, nullptr, pageProtectionMask, 0, 0, nullptr);
	if(fileMappingObject_ == INVALID_HANDLE_VALUE)
	{
		CloseHandle(fileHandle_);
		return nullopt;
	}

	DWORD fileMapAccessMask = 0;
	switch(access & ~EXCLUSIVE)
	{
		case READ:
			fileMapAccessMask = FILE_MAP_READ;
			break;
		case READ | WRITE:
			fileMapAccessMask = FILE_MAP_READ | FILE_MAP_WRITE;
			break;
	}
	file_ = reinterpret_cast<char *>(MapViewOfFile(fileMappingObject_, fileMapAccessMask, 0, 0, 0));
}

MappedFile::MappedFile(MappedFile &&other) noexcept
{
	fileHandle_		   = other.fileHandle_;
	fileMappingObject_ = other.fileMappingObject_;

	file_		= other.file_;
	fileSize_   = other.fileSize_;
	other.file_ = nullptr;
}

MappedFile &MappedFile::operator=(MappedFile &&other) noexcept
{
	UnmapViewOfFile(file_);
	CloseHandle(fileMappingObject_);
	CloseHandle(fileHandle_);
	fileHandle_		   = other.fileHandle_;
	fileMappingObject_ = other.fileMappingObject_;

	file_		= other.file_;
	fileSize_   = other.fileSize_;
	other.file_ = nullptr;

	return *this;
}

MappedFile::~MappedFile()
{
	if(file_)
	{
		UnmapViewOfFile(file_);
		CloseHandle(fileMappingObject_);
		CloseHandle(fileHandle_);
	}
}

}
