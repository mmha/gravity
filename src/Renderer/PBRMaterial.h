#ifndef GE_OGL4_PBRMATERIAL_H
#define GE_OGL4_PBRMATERIAL_H

#include <glbinding/gl45/gl.h>

namespace ge
{
	namespace gl4
	{
		using namespace ::gl;
		template <GLenum textureType> class Texture;
		class Sampler;

		class PBRMaterial
		{
			public:
				enum class TexUnits
				{
					Albedo = 1,
					Normal = 2,
					Roughness = 3
				};

				PBRMaterial(Texture<GL_TEXTURE_2D> &albedo, Texture<GL_TEXTURE_2D> &normal, Texture<GL_TEXTURE_2D> &roughness, Sampler &sampler);

				Texture<GL_TEXTURE_2D> &albedo();
				Texture<GL_TEXTURE_2D> &normal();
				Texture<GL_TEXTURE_2D> &roughness();
				Sampler &sampler();

				void use() const;
			private:
				Texture<GL_TEXTURE_2D> &albedo_;
				Texture<GL_TEXTURE_2D> &normal_;
				Texture<GL_TEXTURE_2D> &roughness_;
				Sampler &sampler_;
		};
	}
}

#endif