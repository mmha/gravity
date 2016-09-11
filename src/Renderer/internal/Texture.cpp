#include "Texture.h"

using namespace gl45;

ge::gl4::Texture<GL_TEXTURE_2D> ge::gl4::loadTexture(const Core::Image & img)
{
	GLenum possibleInternalFormats[]{
		GL_R8,	GL_R32F,		 // Image::Channels::ONE
		GL_RG8,   GL_RG32F,		 // Image::Channels::TWO
		GL_RGB8,  GL_RGB32F,	 // Image::Channels::THREE
		GL_RGBA8, GL_RGBA32F,	// Image::Channels::FOUR
	};
	static_assert(sizeof(possibleInternalFormats) == (size_t)Core::Image::Channels::FOUR * (size_t)Core::Image::PixelFormat::COUNT * sizeof(GLenum),
		"Please update possibleInternalFormats[] in gl4::Texture::fromImage() after adding a new PixelFormat");
	auto internalFormat = possibleInternalFormats[(uint32_t)img.channels() * (uint32_t)Core::Image::PixelFormat::COUNT + (uint32_t)img.format()];

	GLenum possibleFormats[]
	{
		GL_RED, GL_RG, GL_RGB, GL_RGBA
	};
	static_assert(sizeof(possibleFormats) == (size_t)Core::Image::Channels::FOUR * sizeof(GLenum), "Missing Channel translation");
	auto format = possibleFormats[(uint32_t)img.channels() - 1];

	GLenum possibleTypes[]
	{
		GL_UNSIGNED_BYTE, GL_FLOAT
	};
	static_assert(sizeof(possibleTypes) == (size_t)Core::Image::PixelFormat::COUNT * sizeof(GLenum), "Please update possibleTypes[] in gl4::Texture::fromImage() after adding a new PixelFormat");
	auto type = possibleTypes[(uint32_t)img.format()];

	Texture<GL_TEXTURE_2D> tex{ NO_MIPMAPS, internalFormat, img.size() };
	tex.upload(0, format, type, img.data());
	return tex;
}
