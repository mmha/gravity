#pragma once

#include "Shader.h"
#include "../../Core/Enum.h"
#include <memory>

namespace ge::gl4
{
	class ProgramPipeline
	{
	public:
		using ShaderArray = Core::EnumArray<std::shared_ptr<Shader>, Shader::Type>;

		ProgramPipeline();
		ProgramPipeline(const ProgramPipeline &other);
		ProgramPipeline(ProgramPipeline &&other) noexcept;
		ProgramPipeline &operator=(ProgramPipeline &&other) noexcept;
		~ProgramPipeline();

		auto begin() -> ShaderArray::iterator;
		auto end() -> ShaderArray::iterator;
		auto begin() const -> ShaderArray::const_iterator;
		auto end() const  -> ShaderArray::const_iterator;
		auto operator[](Shader::Type type) -> ShaderArray::reference;
		auto operator[](Shader::Type type) const -> ShaderArray::const_reference;

		void attach(std::shared_ptr<Shader> shader);
		
		void use() const;
		auto id() const -> uint32_t;

		auto operator==(const ProgramPipeline &rhs) const -> bool;
		auto operator!=(const ProgramPipeline &rhs) const -> bool;
	private:
		uint32_t pipelineID_;
		ShaderArray attachedShaders_;
	};
}

namespace std
{
	template<>
	struct hash<ge::gl4::ProgramPipeline>
	{
		auto operator()(const ge::gl4::ProgramPipeline &val) const -> size_t
		{
			return val.id();
		}
	};

	template<>
	struct less<ge::gl4::ProgramPipeline>
	{
		auto operator()(const ge::gl4::ProgramPipeline &lhs, const ge::gl4::ProgramPipeline &rhs) const -> bool
		{
			return lhs.id() < rhs.id();
		}
	};
}