#include "../Core/MappedFile.h"
#include "Mesh.h"
#include "Renderer.h"

#include "GeometryData.h"
#include "PBRMaterial.h"
#include "Primitive.h"
#include "Sampler.h"
#include "internal/ProgramPipeline.h"
#include "internal/Shader.h"
#include "internal/Texture.h"
#include "internal/VertexFormat.h"

namespace ge
{
	namespace gl4
	{
		Mesh::Mesh(std::vector<Primitive *> primitives, const lmi::mat4 &transformation)
			: primitives_(std::move(primitives))
			, transformation_(transformation)
		{
		}

		void Mesh::setTransformation(const lmi::mat4 &t)
		{
			transformation_ = t;
		}

		void Mesh::draw() const
		{
			for(auto p : primitives_)
			{
				p->draw();
			}
		}
	}
}