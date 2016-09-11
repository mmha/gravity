#ifndef GE_OGL4_MESH_H
#define GE_OGL4_MESH_H

#include <lmi/lmi.h>
#include <vector>

namespace ge
{
	namespace gl4
	{
		class Renderer;
		class Primitive;
		
		void loadMesh(Renderer &renderer, const std::string &filename);

		class Mesh
		{
		public:
			Mesh(std::vector<Primitive *> primitives, const lmi::mat4 &transformation);
			void draw() const;
			void setTransformation(const lmi::mat4 &t);
		private:
			std::vector<Primitive *> primitives_;
			lmi::mat4 transformation_;
		};
	}
}

#endif