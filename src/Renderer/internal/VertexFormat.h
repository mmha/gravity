#pragma once

#include <glbinding/gl45/gl.h>

namespace ge::gl4
{
	using namespace gl45;
	class Buffer;

	class VertexFormat
	{
	public:
		VertexFormat();
		VertexFormat(const VertexFormat &) = delete; //TODO, this should be possible strictly speaking
		VertexFormat(VertexFormat &&other);
		VertexFormat &operator=(VertexFormat &&other);
		~VertexFormat();
		
		// Attribute Specification
		void setAttributeFormat(uint32_t index, int32_t size, gl::GLenum type, bool normalized, uint32_t relativeOffset);
		void setAttributeEnabled(uint32_t index, bool enabled);
		void setAttributeBinding(uint32_t attributeIndex, uint32_t bindingIndex);
		
		// Storage Specification
		void bindBufferAsAttribute(const Buffer& buffer, uint32_t index, size_t offset, size_t stride);
		void bindBuffersAsAttributes(uint32_t baseAttributeIndex, GLsizei count, const uint32_t *vboIDs, const GLintptr *offsets, const GLsizei *strides);
		void bindIndexBuffer(const Buffer &buffer);

		// Use
		void use() const;
		static void stopUsing();

		auto id() const -> GLuint;
	private:
		uint32_t vaoID_;
	};
}

namespace std
{
	template<>
	struct hash<ge::gl4::VertexFormat>
	{
		auto operator()(const ge::gl4::VertexFormat &format) const -> size_t
		{
			return format.id();
		}
	};

	template<>
	struct less<ge::gl4::VertexFormat>
	{
		auto operator()(const ge::gl4::VertexFormat &lhs, const ge::gl4::VertexFormat &rhs) const -> bool
		{
			return lhs.id() < rhs.id();
		}
	};
}