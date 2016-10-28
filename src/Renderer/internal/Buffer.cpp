#include "Buffer.h"

namespace ge::gl4
{

Buffer::Buffer(size_t size, const void *data, BufferStorageMask mask)
: size_(size)
, storageMask_{mask}
{
	glCreateBuffers(1, &vboID_);
	glNamedBufferStorage(vboID_, size, data, storageMask_);
}

Buffer::Buffer(const Buffer &other)
: Buffer(other.size(), nullptr, other.storageMask_)
{
	glCopyNamedBufferSubData(other.vboID_, vboID_, 0, 0, other.size_);
}

Buffer::Buffer(Buffer &&other) noexcept
{
	vboID_ = other.vboID_;
	size_  = other.size_;
	storageMask_ = other.storageMask_;
	other.vboID_ = 0;
}

Buffer &Buffer::operator=(Buffer &&other) noexcept
{
	glDeleteBuffers(1, &vboID_);
	vboID_ = other.vboID_;
	size_  = other.size_;
	storageMask_ = other.storageMask_;
	other.vboID_ = 0;
	return *this;
}

Buffer::~Buffer()
{
	if(vboID_)
	{
		glDeleteBuffers(1, &vboID_);
	}
}

auto Buffer::map(BufferAccessMask accessFlags) -> void *
{
	return map(accessFlags, 0, size_);
}

auto Buffer::map(BufferAccessMask accessFlags, size_t offset, size_t size) -> void *
{
	return glMapNamedBufferRange(vboID_, offset, size, accessFlags);
}

void Buffer::unmap()
{
	glUnmapNamedBuffer(vboID_);
}

void Buffer::flush()
{
	flush(0, size_);
}

void Buffer::flush(size_t offset, size_t size)
{
	glFlushMappedNamedBufferRange(vboID_, offset, size);
}

void Buffer::upload(const void *data)
{
	upload(0, size_, data);
}

void Buffer::upload(size_t offset, size_t size, const void *data)
{
	glNamedBufferSubData(vboID_, offset, size, data);
}

void Buffer::download(void *destination) const
{
	download(0, size_, destination);
}

void Buffer::download(size_t offset, size_t size, void *destination) const
{
	glGetNamedBufferSubData(vboID_, offset, size, destination);
}

auto Buffer::download() const -> std::vector<uint8_t>
{
	return download(0, size_);
}

auto Buffer::download(size_t offset, size_t size) const -> std::vector<uint8_t>
{
	std::vector<uint8_t> destination(size);
	download(offset, size, destination.data());
	return destination;
}

void Buffer::bind(GLenum target) const
{
	glBindBuffer(target, vboID_);
}

void Buffer::bindToIndex(GLenum target, GLuint index) const
{
	glBindBufferBase(target, index, vboID_);
}

void Buffer::bindToIndex(GLenum target, GLuint index, size_t offset, size_t size) const
{
	glBindBufferRange(target, index, vboID_, offset, size);
}

void Buffer::unbind(GLenum target)
{
	glBindBuffer(target, 0);
}

void Buffer::unbind(GLenum target, GLuint index)
{
	glBindBufferBase(target, index, 0);
}

size_t Buffer::size() const
{
	return size_;
}

auto Buffer::id() const -> GLuint
{
	return vboID_;
}

auto Buffer::operator==(const Buffer &rhs) const -> bool
{
	return download() == rhs.download();
}

auto Buffer::operator!=(const Buffer &rhs) const -> bool
{
	return download() != rhs.download();
}

}