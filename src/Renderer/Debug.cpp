#include <iostream>
#include <unordered_map>
#include <glbinding/gl45/gl.h>
#include "Debug.h"

using namespace gl45;

static std::ostream *debugOutputStream = &std::cerr;

static void onOpenGLDebug(GLenum source,
						  GLenum type,
						  GLuint id,
						  GLenum severity,
						  GLsizei /*length*/,
						  const char* message,
						  const void* /*userParam*/)
{
	static std::unordered_map<GLenum, const std::string> str
	{
		{GL_DEBUG_SOURCE_API, "GL_DEBUG_SOURCE_API"},
		{GL_DEBUG_SOURCE_WINDOW_SYSTEM, "GL_DEBUG_SOURCE_WINDOW_SYSTEM"},
		{GL_DEBUG_SOURCE_SHADER_COMPILER, "GL_DEBUG_SOURCE_SHADER_COMPILER"},
		{GL_DEBUG_SOURCE_THIRD_PARTY, "GL_DEBUG_SOURCE_THIRD_PARTY"},
		{GL_DEBUG_SOURCE_APPLICATION, "GL_DEBUG_SOURCE_APPLICATION"},
		{GL_DEBUG_SOURCE_OTHER, "GL_DEBUG_SOURCE_OTHER"},
		{GL_DEBUG_TYPE_ERROR, "GL_DEBUG_TYPE_ERROR"},
		{GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR, "GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR"},
		{GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR, "GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR"},
		{GL_DEBUG_TYPE_PORTABILITY, "GL_DEBUG_TYPE_PORTABILITY"},
		{GL_DEBUG_TYPE_PERFORMANCE, "GL_DEBUG_TYPE_PERFORMANCE"},
		{GL_DEBUG_TYPE_OTHER, "GL_DEBUG_TYPE_OTHER"},
		{GL_DEBUG_SEVERITY_HIGH, "GL_DEBUG_SEVERITY_HIGH"},
		{GL_DEBUG_SEVERITY_MEDIUM, "GL_DEBUG_SEVERITY_MEDIUM"},
		{GL_DEBUG_SEVERITY_LOW, "GL_DEBUG_SEVERITY_LOW"},
	};
	//*debugOutputStream << message << std::endl; return;
	*debugOutputStream << "{\"source\": \""		<< str[source]		<< "\", "
					   << "\"type\": \""		<< str[type]		<< "\", "
					   << "\"id\": "			<< id				<< ", "
					   << "\"severity\": \""	<< str[severity]	<< "\", "
					   << "\"message\": \""		<< message			<< "\"}" << std::endl;
}

void ge::gl4::enableGLDebugOuput(DebugOutputMode mode)
{
	glDebugMessageCallback(onOpenGLDebug, nullptr);
	glEnable(GL_DEBUG_OUTPUT);
	if(mode == DebugOutputMode::SYNCHRONOUS)
	{
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	}
}

void ge::gl4::setGLDebugOutputStream(std::ostream &stream)
{
	debugOutputStream = &stream;
}

void ge::gl4::disableGLDebugOutput()
{
	glDisable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDisable(GL_DEBUG_OUTPUT);
}