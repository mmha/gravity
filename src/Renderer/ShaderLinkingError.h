#ifndef GE_OGL4_SHADERLINKINGEXCEPTION_H
#define GE_OGL4_SHADERLINKINGEXCEPTION_H

#include <string>
#include <cstdint>
#include <exception>

namespace ge::gl4
{
	class ShaderLinkingError : public std::exception
	{
	public:
		ShaderLinkingError();
		ShaderLinkingError(const uint32_t programID);
		ShaderLinkingError(uint32_t objectID, const std::string& descriptor);
		auto what() const noexcept -> const char* override;
	private:
		std::string msg_;
	};
}

#endif
