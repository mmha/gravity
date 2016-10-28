#include "ProgramPipeline.h"
#include <cassert>
#include <glbinding/gl45/gl.h>

namespace ge::gl4
{

ProgramPipeline::ProgramPipeline()
{
	glCreateProgramPipelines(1, &pipelineID_);
}

ProgramPipeline::ProgramPipeline(const ProgramPipeline &other)
: ProgramPipeline{}
{
	for(auto &s : other)
	{
		attach(s);
	}
}

ProgramPipeline::ProgramPipeline(ProgramPipeline &&other) noexcept
: attachedShaders_(std::move(other.attachedShaders_))
{
	pipelineID_		  = other.pipelineID_;
	other.pipelineID_ = 0;
}

ProgramPipeline &ProgramPipeline::operator=(ProgramPipeline &&other) noexcept
{
	attachedShaders_ = std::move(other.attachedShaders_);
	glDeleteProgramPipelines(1, &pipelineID_);
	pipelineID_		  = other.pipelineID_;
	other.pipelineID_ = 0;
	return *this;
}

ProgramPipeline::~ProgramPipeline()
{
	if(pipelineID_)
	{
		glDeleteProgramPipelines(1, &pipelineID_);
	}
}

auto ProgramPipeline::begin() -> ShaderArray::iterator
{
	return attachedShaders_.begin();
}

auto ProgramPipeline::end() -> ShaderArray::iterator
{
	return attachedShaders_.end();
}

auto ProgramPipeline::begin() const -> ShaderArray::const_iterator
{
	return attachedShaders_.begin();
}

auto ProgramPipeline::end() const -> ShaderArray::const_iterator
{
	return attachedShaders_.end();
}

auto ProgramPipeline::operator[](AttachedShaderIndex type) -> ShaderArray::reference
{
	return attachedShaders_[type];
}

auto ProgramPipeline::operator[](AttachedShaderIndex type) const -> ShaderArray::const_reference
{
	return attachedShaders_[type];
}


void ProgramPipeline::attach(std::shared_ptr<Shader> shader)
{
	const auto type =  shader->type();
	const auto mask = [type]() -> std::pair<UseProgramStageMask, AttachedShaderIndex>
	{
		switch(type)
		{
			case Shader::Type::Vertex:                 return {GL_VERTEX_SHADER_BIT,          AttachedShaderIndex::Vertex};
			case Shader::Type::Fragment:               return {GL_FRAGMENT_SHADER_BIT,        AttachedShaderIndex::Fragment};
			case Shader::Type::Geometry:               return {GL_GEOMETRY_SHADER_BIT,        AttachedShaderIndex::Geometry};
			case Shader::Type::TessellationControl:    return {GL_TESS_CONTROL_SHADER_BIT,    AttachedShaderIndex::TessellationControl};
			case Shader::Type::TessellationEvaluation: return {GL_TESS_EVALUATION_SHADER_BIT, AttachedShaderIndex::TessellationEvaluation};
			case Shader::Type::Compute:                return {GL_COMPUTE_SHADER_BIT,         AttachedShaderIndex::Compute};
			default: throw std::invalid_argument{"Unknown Shader Type"};
		}
	}();

	glUseProgramStages(pipelineID_, mask.first, shader->id());
	attachedShaders_[mask.second] = std::move(shader);
}

void ProgramPipeline::use() const
{
	glBindProgramPipeline(pipelineID_);
}

auto ProgramPipeline::id() const -> GLuint
{
	return pipelineID_;
}

auto ProgramPipeline::operator==(const ProgramPipeline &rhs) const -> bool
{
	return attachedShaders_ == rhs.attachedShaders_;
}

auto ProgramPipeline::operator!=(const ProgramPipeline &rhs) const -> bool
{
	return attachedShaders_ != rhs.attachedShaders_;
}

}
