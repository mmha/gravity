#pragma once

#include <glbinding/gl45/gl.h>
#include <vector>

namespace ge::gl4
{
	using namespace gl45;

	class Buffer
	{
	public:
		explicit Buffer(size_t size, const void *data = nullptr);
		Buffer(const Buffer &other);
		Buffer(Buffer &&other) noexcept;
		Buffer &operator=(Buffer &&other) noexcept;
		~Buffer();
		
		// Mapping Functions
		auto map(BufferAccessMask accessFlags) -> void *;
		auto map(BufferAccessMask accessFlags, size_t offset, size_t size) -> void *;
		void flush();
		void flush(size_t offset, size_t size);
		void unmap();
		
		// Data Transfer Functions
		void upload(const void *data);
		void upload(size_t offset, size_t size, const void *data);
		void download(void *destination) const;
		void download(size_t offset, size_t size, void *destination) const;
		auto download() const -> std::vector<uint8_t>;
		auto download(size_t offset, size_t size) const -> std::vector<uint8_t>;
		
		//Bindings
		void bind(GLenum target) const;
		void bindToIndex(GLenum target, uint32_t index) const;
		void bindToIndex(GLenum target, uint32_t index, size_t offset, size_t size) const;
		static void unbind(GLenum target);
		static void unbind(GLenum target, uint32_t index);
		
		auto size() const -> size_t;
		auto id() const -> uint32_t;

		// Comparision through download() - expensive operations!
		auto operator==(const Buffer &rhs) const -> bool;
		auto operator!=(const Buffer &rhs) const -> bool;
	protected:
		uint32_t vboID_;
		size_t size_;
	};
}

namespace std
{
	template<>
	struct hash<::ge::gl4::Buffer>
	{
		auto operator()(const ::ge::gl4::Buffer &buffer) const -> size_t
		{
			return buffer.id();
		};
	};

	template<>
	struct less<::ge::gl4::Buffer>
	{
		auto operator()(const ::ge::gl4::Buffer &lhs, const ::ge::gl4::Buffer &rhs) const -> bool
		{
			return lhs.id() < rhs.id();
		}
	};
}