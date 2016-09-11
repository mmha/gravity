#include "VertexFormat.h"
#include "Buffer.h"

using namespace ge::gl4;

VertexFormat::VertexFormat()
{
	glCreateVertexArrays(1, &vaoID_);
}

VertexFormat::VertexFormat(VertexFormat &&other)
{
	vaoID_ = other.vaoID_;
	other.vaoID_ = 0;
}

VertexFormat &VertexFormat::operator=(VertexFormat &&other)
{
	glDeleteVertexArrays(1, &vaoID_);
	vaoID_ = other.vaoID_;
	other.vaoID_ = 0;
	return *this;
}

VertexFormat::~VertexFormat()
{
	if(vaoID_)
	{
		glDeleteVertexArrays(1, &vaoID_);
	}
}

void VertexFormat::setAttributeFormat(uint32_t index, int32_t size, GLenum type, bool normalized, uint32_t relativeOffset)
{
	glVertexArrayAttribFormat(vaoID_, index, size, type, (GLboolean) normalized, relativeOffset);
}

void VertexFormat::setAttributeEnabled(uint32_t index, bool enabled)
{
	if(enabled)
	{
		glEnableVertexArrayAttrib(vaoID_, index);
	}
	else
	{
		glDisableVertexArrayAttrib(vaoID_, index);
	}
}

void ge::gl4::VertexFormat::setAttributeBinding(uint32_t attributeIndex, uint32_t bindingIndex)
{
	glVertexArrayAttribBinding(vaoID_, attributeIndex, bindingIndex);
}

void VertexFormat::bindBufferAsAttribute(const Buffer& buffer, uint32_t index, size_t offset, size_t stride)
{
	glVertexArrayVertexBuffer(vaoID_, index, buffer.id(), offset, (GLsizei) stride);
}

void VertexFormat::bindBuffersAsAttributes(uint32_t baseAttributeIndex, GLsizei count, const uint32_t *vboIDs, const GLintptr *offsets, const GLsizei *strides)
{
	glVertexArrayVertexBuffers(vaoID_, baseAttributeIndex, count, vboIDs, offsets, strides);
}

void ge::gl4::VertexFormat::bindIndexBuffer(const Buffer & buffer)
{
	glVertexArrayElementBuffer(vaoID_, buffer.id());
}

void VertexFormat::use() const
{
	glBindVertexArray(vaoID_);
}

void VertexFormat::stopUsing()
{
	glBindVertexArray(0);
}

GLuint VertexFormat::id() const
{
	return vaoID_;
}