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

auto ProgramPipeline::begin() -> ProgramPipeline::ShaderArray::iterator
{
	return attachedShaders_.begin();
}

auto ProgramPipeline::end() -> ProgramPipeline::ShaderArray::iterator
{
	return attachedShaders_.end();
}

auto ProgramPipeline::begin() const -> ProgramPipeline::ShaderArray::const_iterator
{
	return attachedShaders_.begin();
}

auto ProgramPipeline::end() const -> ProgramPipeline::ShaderArray::const_iterator
{
	return attachedShaders_.end();
}

auto ProgramPipeline::operator[](Shader::Type type) -> ProgramPipeline::ShaderArray::reference
{
	return attachedShaders_[type];
}

auto ProgramPipeline::operator[](Shader::Type type) const -> ProgramPipeline::ShaderArray::const_reference
{
	return attachedShaders_[type];
}


void ProgramPipeline::attach(std::shared_ptr<Shader> shader)
{
	const auto type =  shader->type();
	const auto mask = [type]
	{
		switch(type)
		{
			case Shader::Type::Vertex:                 return GL_VERTEX_SHADER_BIT;
			case Shader::Type::Fragment:               return GL_FRAGMENT_SHADER_BIT;
			case Shader::Type::Geometry:               return GL_GEOMETRY_SHADER_BIT;
			case Shader::Type::TessellationControl:    return GL_TESS_CONTROL_SHADER_BIT;
			case Shader::Type::TessellationEvaluation: return GL_TESS_EVALUATION_SHADER_BIT;
			case Shader::Type::Compute:                return GL_COMPUTE_SHADER_BIT;
			default: throw std::invalid_argument{"Unknown Shader Type"};
		}
	}();

	glUseProgramStages(pipelineID_, mask, shader->id());
	//attachedShaders_[type] = std::move(shader);
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
