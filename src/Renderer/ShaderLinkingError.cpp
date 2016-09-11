#include <glbinding/gl45/gl.h>
#include "ShaderLinkingError.h"
#include <memory>

using namespace gl;

namespace ge::gl4
{

ShaderLinkingError::ShaderLinkingError() : msg_("Unknown Shader Linking Error") {}

ShaderLinkingError::ShaderLinkingError(uint32_t programID)
{
	GLint logLen;
	glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &logLen);
	if (logLen)
	{
		msg_.resize(logLen);
		GLsizei written;
		glGetProgramInfoLog(programID, logLen, &written, const_cast<char *>(msg_.c_str()));
	}
	else
	{
		msg_ = "Unknown Shader Linking Error";
	}
}

ShaderLinkingError::ShaderLinkingError(uint32_t objectID, const std::string& descriptor)
	: ShaderLinkingError(objectID)
{
	msg_ = descriptor + msg_;
}

auto ShaderLinkingError::what() const noexcept -> const char*
{
	return msg_.c_str();
}

}
