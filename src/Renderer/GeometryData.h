#ifndef GE_OGL4_GEOMETRYDATA
#define GE_OGL4_GEOMETRYDATA

#include <vector>
#include <utility>
#include "internal/Buffer.h"

namespace ge
{
	namespace gl4
	{
		using namespace gl45;
		class VertexFormat;

		struct VertexAttribute
		{
			Buffer buffer;
			uint32_t attributeSize;
		};

		class GeometryData
		{
		public:
			GeometryData(std::vector<VertexAttribute> &&vertices, Buffer &&indices, VertexFormat &vertexFormat, GLenum indexFormat, GLenum primitiveType);

			std::vector<VertexAttribute> &vertices();
			Buffer &indices();
			VertexFormat &vertexFormat();
			GLenum indexFormat();
			GLenum primitiveType();

			void draw() const;
		private:
			std::vector<VertexAttribute> vertices_;
			Buffer indices_;
			VertexFormat &vertexFormat_;
			GLenum indexFormat_;
			GLenum primitiveType_;
		};
	}
}

#endif