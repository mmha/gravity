#include "ShaderDefinitions.h"

namespace ge::gl4
{

std::vector<std::string> vertexAttributes;

const std::experimental::string_view shaderDefinitions = 
		R"(#version 330 core)"
		R"(#extension GL_ARB_explicit_uniform_location: require)"
		R"(#extension GL_ARB_separate_shader_objects: require)"
		R"(#extension GL_ARB_shading_language_420pack: require)"
		R"(#extension GL_ARB_enhanced_layouts: require)"
		R"(#define GE_ULOCATION_CAMERA 0)"
		R"(#define GE_ULOCATION_MODELINFO 1)"
		R"(#define GE_ULOCATION_TECHNIQUE_PARAMETER 2)"
		"\n"
		R"(layout(std140, location = GE_ULOCATION_CAMERA) uniform Camera)"
		R"({)"
		R"(	mat4 projMat;)"
		R"(	mat4 viewMat;)"
		R"(} camera;)"
		"\n"
		R"(layout(std140, location = GE_ULOCATION_MODELINFO) uniform ModelInfo)"
		R"({)"
		R"(	mat4 modelMat;)"
		R"(} modelInfo;)"
		"\n";
		
const Core::EnumArray<std::experimental::string_view, UniformLocations> builtinUniformNames
{
	"Camera", "ModelInfo", "TechniqueParameter"
};

}
