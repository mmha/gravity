#pragma once
#include <array>
#include <vector>
#include <cstdint>
#include <experimental/string_view>
#include "../../Core/Enum.h"

namespace ge::gl4
{
	enum class UniformLocations : uint32_t
	{
		GE_ULOCATION_CAMERA,
		GE_ULOCATION_MODELINFO,
		GE_ULOCATION_TECHNIQUE_PARAMETER,
		COUNT
	};

	constexpr auto shaderDefinitions = std::experimental::string_view
	{/*
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
		"\n"*/
	};

	constexpr auto builtinUniformNames = Core::EnumArray<std::experimental::string_view, UniformLocations>
	{
		//"Camera", "ModelInfo", "TechniqueParameter"
	};
	extern std::vector<std::string> vertexAttributes; //TODO flat_set
}

//GE_DECLARE_ENUM_STD_HASHER(::ge::gl4::UniformLocations)
