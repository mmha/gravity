#include "GeometryData.h"
#include "internal/VertexFormat.h"

namespace ge
{
	namespace gl4
	{

		GeometryData::GeometryData(std::vector<VertexAttribute> &&vertices, Buffer &&indices, VertexFormat &vertexFormat, GLenum indexFormat, GLenum primitiveType)
		: vertices_(std::move(vertices))
		, indices_(std::move(indices))
		, vertexFormat_(vertexFormat)
		, indexFormat_(indexFormat)
		, primitiveType_(primitiveType)
		{
			
		}

		std::vector<VertexAttribute> &GeometryData::vertices()
		{
			return vertices_;
		}

		Buffer &GeometryData::indices()
		{
			return indices_;
		}

		VertexFormat &GeometryData::vertexFormat()
		{
			return vertexFormat_;
		}

		GLenum GeometryData::indexFormat()
		{
			return indexFormat_;
		}

		GLenum GeometryData::primitiveType()
		{
			return primitiveType_;
		}

		void GeometryData::draw() const
		{
			for(uint32_t i = 0; i < vertices_.size(); ++i)
			{
				vertexFormat_.bindBufferAsAttribute(vertices_[i].buffer, i, 0, vertices_[i].attributeSize);
			}
			vertexFormat_.bindIndexBuffer(indices_);
			vertexFormat_.use();
			glDrawElements(primitiveType_, indices_.size() / 2, indexFormat_, nullptr);
		}
	}
}