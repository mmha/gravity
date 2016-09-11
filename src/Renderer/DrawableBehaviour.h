#ifndef GE_DRAWABLEBEHAVIOUR_H
#define GE_DRAWABLEBEHAVIOUR_H

#include "../Core/Entity.h"
#include "Mesh.h"

namespace ge
{
	namespace gl4
	{
		class Renderer;
		
		GE_DECLARE_ATTRIBUTE_ACCESSOR(Mesh, mesh)

		class DrawableBehaviour
		{
		public:
			DrawableBehaviour(Mesh &&mesh, Renderer &renderer);
			void onUpdate(float dt);
			void setParent(Core::EntityBase *parent);
		private:
			Renderer &renderer_;
			Mesh mesh_;
		};
	}
}

#endif