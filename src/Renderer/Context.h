#pragma once

#include "../Core/Sigslot.h"
#include <lmi/lmi.h>

namespace ge
{
	namespace gl4
	{
		template<typename C>
		concept bool Context =
		requires(C c)
		{
			{c.makeCurrent()} -> void;
			{c.swapBuffers()} -> void;
			{c.getNativeHandle()};
			{c.getSize()} -> lmi::vec2ui;
			{c.resized} -> Core::Signal<Core::single_threaded, lmi::vec2ui>;
		};
	}
}
