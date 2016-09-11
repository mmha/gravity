#ifndef GE_OGL4_DEBUG_H
#define GE_OGL4_DEBUG_H

#include <ostream>

namespace ge
{
	namespace gl4
	{	
		enum class DebugOutputMode
		{
			SYNCHRONOUS, ASYNCHRONOUS
		};

		void setGLDebugOutputStream(std::ostream &stream);
		void enableGLDebugOuput(DebugOutputMode mode = DebugOutputMode::ASYNCHRONOUS);
		void disableGLDebugOutput();
		
	}
}
#endif