#pragma once
#include "../ShaderLinkingError.h"
#include <glbinding/gl45/gl.h>
#include <experimental/string_view>

namespace ge::gl4
{
	using namespace gl45;
	class Shader
	{
	public:
		enum class Type : uint32_t
		{
			Vertex                 = static_cast<uint32_t>(gl::GL_VERTEX_SHADER),
			Fragment               = static_cast<uint32_t>(gl::GL_FRAGMENT_SHADER),
			Geometry               = static_cast<uint32_t>(gl::GL_GEOMETRY_SHADER),
			TessellationControl    = static_cast<uint32_t>(gl::GL_TESS_CONTROL_SHADER),
			TessellationEvaluation = static_cast<uint32_t>(gl::GL_TESS_EVALUATION_SHADER),
			Compute                = static_cast<uint32_t>(gl::GL_COMPUTE_SHADER),
			COUNT                  = 6
		};

		Shader(Type type, std::experimental::string_view source);
		Shader(const Shader &) = delete;
		Shader(Shader &&other) noexcept;
		Shader &operator=(Shader &&other) noexcept;
		~Shader();

		auto getUniformBlockIndex(std::experimental::string_view name) const -> uint32_t;

		void setUniformBlockBinding(uint32_t index, uint32_t binding);
		void setUniformBlockBinding(std::experimental::string_view name, uint32_t binding);
		
		// Getters
		auto id() const -> uint32_t;
		auto type() const -> Type;
	private:
		GLuint programID_;
		Type type_;
	};
}

namespace std
{
	template<>
	struct hash<ge::gl4::Shader>
	{
		auto operator()(const ge::gl4::Shader &shader) const -> size_t
		{
			return shader.id();
		}
	};

	template<>
	struct less<ge::gl4::Shader>
	{
		auto operator()(const ge::gl4::Shader &lhs, const ge::gl4::Shader &rhs) const -> bool
		{
			return lhs.id() < rhs.id();
		}
	};
}
