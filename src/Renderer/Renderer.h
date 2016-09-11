#pragma once

#include "Context.h"

#include "internal/Buffer.h"
#include "internal/Shader.h"
#include "internal/ProgramPipeline.h"
#include "Sampler.h"
#include "internal/Texture.h"
#include "PBRMaterial.h"
#include "internal/VertexFormat.h"
#include "GeometryData.h"
#include "Primitive.h"
#include "../Core/Entity.h"
#include <glbinding/Binding.h>

namespace ge::Core
{
class EntityBase;
}

namespace ge::gl4
{
	using namespace gl45;
	using namespace ge::Core::literals;

	template<Context C>
	class Renderer
	{
		//friend void loadMesh(Renderer &renderer, const std::string &filename);

		public:
		Renderer(C &context)
		{
			[[maybe_unused]] const static bool once = []
			{
				glbinding::Binding::initialize();
				return true;
			}();

			context_ = &context;

			//glEnable(GL_DEPTH_TEST);
			//glEnable(GL_CULL_FACE);
			//glCullFace(GL_BACK);
			glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		}

		//Renderer(const Renderer &other) = delete;

		void cycle([[maybe_unused]] Core::EntityBase &root, [[maybe_unused]] float dt)
		{
			glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

			root.onMessage("draw"_msg, nullptr);

			context_->swapBuffers();
		}
		// void enqueue(const MeshDrawCommand &command);

		C &context()
		{
			return context_;
		}

		private:
		C *context_;

		void onResize(lmi::vec2ui newSize)
		{
			glViewport(0, 0, newSize.x, newSize.y);
		}

		enum BufferAccessPattern
		{
			Constant,
			Immutable,
			Dynamic,
			COUNT
		};
		// std::vector<Buffer> buffers[BufferAccessPattern::COUNT];

		//std::unordered_map<std::string, Buffer> buffers_;
		//std::unordered_map<std::string, Shader> shaders_;
		//std::unordered_map<std::string, ProgramPipeline> pipelines_;
		//std::unordered_map<std::string, Sampler> samplers_;
		//std::unordered_map<std::string, Texture<GL_TEXTURE_2D>> textures_;
		//std::unordered_map<std::string, PBRMaterial> materials_;
		//std::unordered_map<std::string, VertexFormat> vertexFormats_;
		//std::unordered_map<std::string, GeometryData> geometryData_;
		//std::unordered_map<std::string, Primitive> primitives_;
		//std::unordered_map<std::string, Mesh> meshes_;
	};
}
