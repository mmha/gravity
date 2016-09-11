#pragma once

#include "internal/Buffer.h"
#include <gsl/span>

namespace ge::gl4
{

using namespace gl45;

class BufferView
{
public:
	BufferView(Buffer &buffer, GLenum type, gsl::span<uint8_t> range);

	// Mapping Functions
	auto map(BufferAccessMask accessFlags) -> void *;
	void flush();
	void unmap();

	void bindToIndex(GLuint index);
	static void unbind(GLenum target, GLuint index);

	auto getBuffer() const -> Buffer &;

	const GLenum type;
	const gsl::span<uint8_t> range;
private:
	Buffer *buffer_;
};

}