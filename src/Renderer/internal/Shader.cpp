#include "Shader.h"
#include "ShaderDefinitions.h"
#include <cstring>
#include <cassert>
#include <iostream>
#include <sstream>
using namespace std::literals;

namespace ge::gl4
{

Shader::Shader(Type type, std::experimental::string_view source)
{
	type_ = type;
	programID_ = glCreateProgram();

	switch(type)
	{
		case Type::Vertex: 					type = static_cast<Type>(GL_VERTEX_SHADER); break;
		case Type::Fragment: 				type = static_cast<Type>(GL_FRAGMENT_SHADER); break;
		case Type::Geometry: 				type = static_cast<Type>(GL_GEOMETRY_SHADER); break;
		case Type::TessellationControl: 	type = static_cast<Type>(GL_TESS_CONTROL_SHADER); break;
		case Type::TessellationEvaluation: 	type = static_cast<Type>(GL_TESS_EVALUATION_SHADER); break;
		case Type::Compute: 				type = static_cast<Type>(GL_COMPUTE_SHADER); break;
		default: throw std::invalid_argument{"Unknown Shader Type"};
	}

	//std::stringstream vertexAttribStringStream;
	//for(size_t i = 0; i < vertexAttributes.size(); ++i)
	//{
	//	vertexAttribStringStream << "#define " << vertexAttributes[i] << " " << i << std::endl;
	//}

	//auto vertexAttribString = vertexAttribStringStream.str();
	auto sourcePtr = std::vector<const char *>(1);//vertexAttributes.size() + 3);
	//sourcePtr.emplace_back(shaderDefinitions.data());
	//sourcePtr.emplace_back(vertexAttribString.c_str());
	sourcePtr.emplace_back(source.data());

	//for(size_t i = 0; i < vertexAttributes.size(); ++i)
	//{
	//	sourcePtr[i + 3] = vertexAttributes[i].c_str();
	//}
	auto ptr = source.data();
	programID_ = glCreateShaderProgramv(static_cast<gl::GLenum>(type), 1, &ptr);

	GLint result;
	glGetProgramiv(programID_, GL_LINK_STATUS, &result);
	if(static_cast<GLboolean>(result) == GL_FALSE)
	{
		//throw ShaderLinkingError(programID_, std::string(shaderDefinitions) + vertexAttribString + std::string(source) + "\n");
		throw ShaderLinkingError(programID_, std::string(source) + "\n");
	}

	for (uint32_t i = 0; i < (uint32_t)UniformLocations::COUNT; ++i)
	{
		//setUniformBlockBinding(builtinUniformNames[i], i);
	}
}

Shader::Shader(Shader &&other) noexcept
{
	type_ = other.type_;
	programID_ = other.programID_;
	other.programID_ = 0;
}

Shader &Shader::operator=(Shader &&other) noexcept
{
	glDeleteProgram(programID_);
	type_ = other.type_;
	programID_ = other.programID_;
	other.programID_ = 0;
	return *this;
}

Shader::~Shader()
{
	if(programID_)
	{
		glDeleteProgram(programID_);
	}
}

auto Shader::getUniformBlockIndex(std::experimental::string_view name) const -> GLuint
{
	return glGetUniformBlockIndex(programID_, name.data());
}

void Shader::setUniformBlockBinding(uint32_t index, uint32_t binding)
{
	glUniformBlockBinding(programID_, index, binding);
}

void Shader::setUniformBlockBinding(std::experimental::string_view name, uint32_t binding)
{
	auto index = getUniformBlockIndex(name.data());
	setUniformBlockBinding(index, binding);
}

auto Shader::id() const -> GLuint
{
	return programID_;
}

auto Shader::type() const -> Shader::Type
{
	return type_;
}

}
