#include "../MappedFile.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <cstring>
#include <stdexcept>

namespace ge::Core
{

MappedFile::MappedFile(const char *filename, Access access, UsageHint usageHint)
{
	if(access & WRITE && !(access & READ))
	{
		throw std::invalid_argument{"Invalid access bit set"};
	}

	int accessMask = 0;
	switch(access & (READ | WRITE))
	{
		case READ:
			accessMask |= O_RDONLY;
			break;
		case WRITE:
			accessMask |= O_WRONLY;
			break;
		case READ | WRITE:
			accessMask |= O_RDWR;
			break;
		default:
			throw std::invalid_argument{"Invalid access bit set"};
	}

	struct stat s;
	fd_ = open(filename, accessMask);
	if(fd_ < 0)
	{
		throw std::runtime_error(strerror(errno));
	}

	fstat(fd_, &s);

	if(access & EXCLUSIVE)
	{
		if(fcntl(fd_, F_SETLK) < 0)
		{
			close(fd_);
			throw std::runtime_error(strerror(errno));
		}
	}

	fileSize_ = s.st_size;

	int protMask = 0;
	if(access & READ)
		protMask |= PROT_READ;
	if(access & WRITE)
		protMask |= PROT_WRITE;
	int flagMask = 0;
	if(access & WRITE)
		flagMask = MAP_SHARED;
	else
		flagMask = MAP_PRIVATE;

	file_ = (char *) mmap(nullptr, fileSize_, protMask, flagMask, fd_, 0);
	if(file_ == MAP_FAILED)
	{
		close(fd_);
		throw std::runtime_error(strerror(errno));
	}

	int advice;
	switch(usageHint)
	{
		case NORMAL:
			advice = MADV_NORMAL;
			break;
		case SEQUENTIAL:
			advice = MADV_SEQUENTIAL;
			break;
		case RANDOM_ACCESS:
			advice = MADV_RANDOM;
			break;
		default:
			advice = MADV_NORMAL;
			break;
	}
	madvise(file_, fileSize_, advice);
}

MappedFile::MappedFile(MappedFile &&other) noexcept
{
	fd_ = other.fd_;
	file_		= other.file_;
	fileSize_   = other.fileSize_;
	other.file_ = nullptr;
}

MappedFile &MappedFile::operator=(MappedFile &&other) noexcept
{
	munmap(file_, fileSize_);
	close(fd_);

	fd_         = other.fd_;
	file_		= other.file_;
	fileSize_   = other.fileSize_;
	other.file_ = nullptr;

	return *this;
}

MappedFile::~MappedFile()
{
	if(file_)
	{
		munmap(file_, fileSize_);
		close(fd_);
	}
}

}
