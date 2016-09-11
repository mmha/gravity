#ifndef GE_OGL4_PRIMITIVE_H
#define GE_OGL4_PRIMITIVE_H

#include <glbinding/gl45/gl.h>

namespace ge
{
	namespace gl4
	{
		using namespace ::gl;
		class GeometryData;
		class PBRMaterial;

		class Primitive
		{
			public:
				Primitive(GeometryData &geometryData, PBRMaterial &material);

				GeometryData &geometryData();
				PBRMaterial &material();
				void draw() const;
		private:
			GeometryData &geometryData_;
			PBRMaterial &material_;
		};
	}
}

#endif //GE_OGL4_PRIMITIVE_H
