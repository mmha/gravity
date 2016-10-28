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

	extern const std::experimental::string_view shaderDefinitions; 

	extern const Core::EnumArray<std::experimental::string_view, UniformLocations> builtinUniformNames; 

	extern std::vector<std::string> vertexAttributes; //TODO flat_set
}

//GE_DECLARE_ENUM_STD_HASHER(::ge::gl4::UniformLocations)
