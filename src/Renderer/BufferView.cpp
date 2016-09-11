#include "BufferView.h"

namespace ge::gl4
{

BufferView::BufferView(Buffer &buffer, GLenum type, gsl::span<uint8_t> range)
: type{type}
, range(range)
, buffer_{&buffer}
{
}

auto BufferView::map(BufferAccessMask accessFlags) -> void *
{
	return buffer_->map(accessFlags, (size_t) range.data(), (size_t) range.data() + range.size());
}

void BufferView::flush()
{
	buffer_->flush((size_t) range.data(), (size_t) range.data() + range.size());
}

void BufferView::unmap()
{
	buffer_->unmap();
}

void BufferView::bindToIndex(GLuint index)
{
	buffer_->bindToIndex(type, index, (size_t) range.data(), (size_t) range.data() + range.size());
}

void BufferView::unbind(GLenum target, GLuint index)
{
	Buffer::unbind(target, index);
}

Buffer &BufferView::getBuffer() const
{
	return *buffer_;
}

}
