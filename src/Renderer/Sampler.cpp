#include "Sampler.h"
using namespace ge;
using namespace ge::gl4;

Sampler::Sampler()
{
	glGenSamplers(1, &samplerID_);
}

Sampler::Sampler(Sampler &&other)
{
	samplerID_ = other.samplerID_;
	other.samplerID_ = 0;
}

Sampler &Sampler::operator=(Sampler &&other)
{
	glDeleteSamplers(1, &samplerID_);
	samplerID_ = other.samplerID_;
	other.samplerID_ = 0;
	return *this;
}

Sampler::~Sampler()
{
	if(samplerID_)
	{
		glDeleteSamplers(1, &samplerID_);
	}
}

void Sampler::set(GLenum parameterName, GLfloat value)
{
	glSamplerParameterf(samplerID_, parameterName, value);
}

void Sampler::set(GLenum parameterName, GLint value)
{
	glSamplerParameteri(samplerID_, parameterName, value);
}

void Sampler::set(GLenum parameterName, GLenum value)
{
	glSamplerParameteri(samplerID_, parameterName, (GLint)value);
}

void Sampler::set(GLenum parameterName, const GLfloat *values)
{
	glSamplerParameterfv(samplerID_, parameterName, values);
}

void Sampler::set(GLenum parameterName, const GLint *values)
{
	glSamplerParameteriv(samplerID_, parameterName, values);
}

void Sampler::set(GLenum parameterName, const GLuint *values)
{
	glSamplerParameterIuiv(samplerID_, parameterName, values);
}

void Sampler::use(GLuint textureUnit) const
{
	glBindSampler(textureUnit, samplerID_);
}

GLuint Sampler::id() const
{
	return samplerID_;
}