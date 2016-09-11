#ifndef GE_OGL4_TEXTURE_H
#define GE_OGL4_TEXTURE_H

#include <glbinding/gl45/gl.h>
#include <lmi/lmi.h>
#include <type_traits>

#include "../../Core/Image.h"

namespace ge::gl4
{
	using namespace gl45;
	class Buffer;

	struct Mipmaps
	{
		constexpr explicit Mipmaps(GLint levels)
			: levels(levels)
		{
		}
		GLint levels;
	};

	const static Mipmaps NO_MIPMAPS = Mipmaps{1};

	namespace internal
	{
		constexpr bool is1D(GLenum type)
		{
			return type == GL_TEXTURE_1D;
		}

		constexpr bool is2D(GLenum type)
		{
			return type == GL_TEXTURE_2D || type == GL_TEXTURE_CUBE_MAP_POSITIVE_X || type == GL_TEXTURE_CUBE_MAP_NEGATIVE_X ||
				   type == GL_TEXTURE_CUBE_MAP_POSITIVE_Y || type == GL_TEXTURE_CUBE_MAP_NEGATIVE_Y || type == GL_TEXTURE_CUBE_MAP_POSITIVE_Z ||
				   type == GL_TEXTURE_CUBE_MAP_NEGATIVE_Z || type == GL_TEXTURE_1D_ARRAY;
		}

		constexpr bool is3D(GLenum type)
		{
			return type == GL_TEXTURE_3D || type == GL_TEXTURE_2D_ARRAY;
		}
	}
	using namespace internal;

	template <GLenum textureType>
	class Texture
	{
		public:
		template <GLenum T = textureType, typename = typename std::enable_if<is1D(T)>::type>
		Texture(Mipmaps m, GLenum internalFormat, lmi::Vector<1, unsigned> size)
			: size_(size[0], 0, 0)
		{
			glCreateTextures(textureType, 1, &texID_);
			glTextureStorage1D(texID_, m.levels, internalFormat, size[0]);
		}

		template <GLenum T = textureType, typename = typename std::enable_if<is2D(T)>::type>
		Texture(Mipmaps m, GLenum internalFormat, lmi::vec2ui size)
			: size_(size[0], size[1], 0)
		{
			glCreateTextures(textureType, 1, &texID_);
			glTextureStorage2D(texID_, m.levels, internalFormat, size[0], size[1]);
		}

		template <GLenum T = textureType, typename = typename std::enable_if<is3D(T)>::type>
		Texture(Mipmaps m, GLenum internalFormat, lmi::vec3ui size)
			: size_(size)
		{
			glCreateTextures(textureType, 1, &texID_);
			glTextureStorage3D(texID_, m.levels, internalFormat, size[0], size[1], size[2]);
		}

		Texture(const Texture &other) = delete;	// TODO this should be possible (but not necessary)

		Texture(Texture &&other)
		{
			texID_		 = other.texID_;
			size_		 = other.size_;
			other.texID_ = 0;
		}

		Texture &operator=(Texture &&other)
		{
			glDeleteTextures(1, &texID_);
			texID_		 = other.texID_;
			size_		 = other.size_;
			other.texID_ = 0;
			return *this;
		}

		~Texture()
		{
			if(texID_)
			{
				glDeleteTextures(1, &texID_);
			}
		}

		// Data Transfer Functions
		template <GLenum T = textureType>
		typename std::enable_if_t<is1D(T)> upload(GLint level, GLenum format, GLenum type, const void *pixels)
		{
			glTextureSubImage1D(texID_, level, 0, size_[0], format, type, pixels);
		}

		template <GLenum T = textureType>
		typename std::enable_if_t<is2D(T)> upload(GLint level, GLenum format, GLenum type, const void *pixels)
		{
			glTextureSubImage2D(texID_, level, 0, 0, size_[0], size_[1], format, type, pixels);
		}

		template <GLenum T = textureType>
		typename std::enable_if_t<is3D(T)> upload(GLint level, GLenum format, GLenum type, const void *pixels)
		{
			glTextureSubImage3D(texID_, level, 0, 0, 0, size_[0], size_[1], size_[2], format, type, pixels);
		}

		// Texture Parameters
		void setTextureParameter(GLenum parameterName, GLfloat value)
		{
			glTextureParameterf(texID_, parameterName, value);
		}

		void setTextureParameter(GLenum parameterName, GLint value)
		{
			glTextureParameteri(texID_, parameterName, value);
		}

		// Is this overload correct?
		void setTextureParameter(GLenum parameterName, GLenum value)
		{
			glTextureParameteri(texID_, parameterName, (GLint)value);
		}

		void setTextureParameter(GLenum parameterName, const GLfloat *values)
		{
			glTextureParameterfv(texID_, parameterName, values);
		}

		void setTextureParameter(GLenum parameterName, const GLint *values)
		{
			glTextureParameteriv(texID_, parameterName, values);
		}

		void setTextureParameter(GLenum parameterName, const GLuint *values)
		{
			glTextureParameterIuiv(texID_, parameterName, values);
		}

		// Bindings
		void bindAsTexture(GLuint textureUnit) const
		{
			glBindTextureUnit(textureUnit, texID_);
		}

		[[deprecated("Stub - not implemented!")]] void bindAsImage(GLuint imageUnit)
		{
			(void)imageUnit;
		}

		// Buffer Textures
		[[deprecated("Stub - not implemented!")]] static Texture fromBuffer(const Buffer &buffer)
		{
			(void)buffer;
		}

		GLenum type()
		{
			return textureType;
		}

		private:
		GLuint texID_;
		lmi::vec3ui size_;
	};

	Texture<GL_TEXTURE_2D> loadTexture(const Core::Image &img);
}

#endif