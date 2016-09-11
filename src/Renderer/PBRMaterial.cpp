#include "PBRMaterial.h"
#include "internal/Texture.h"
#include "Sampler.h"

namespace ge
{
	namespace gl4
	{
		
		PBRMaterial::PBRMaterial(Texture<GL_TEXTURE_2D> &albedo, Texture<GL_TEXTURE_2D> &normal, Texture<GL_TEXTURE_2D> &roughness, Sampler &sampler)
		: albedo_(albedo)
		, normal_(normal)
		, roughness_(roughness)
		, sampler_(sampler)
		{

		}

		Texture<GL_TEXTURE_2D> &PBRMaterial::albedo()
		{
			return albedo_;
		}

		Texture<GL_TEXTURE_2D> &PBRMaterial::normal()
		{
			return normal_;
		}

		Texture<GL_TEXTURE_2D> &PBRMaterial::roughness()
		{
			return roughness_;
		}

		Sampler &PBRMaterial::sampler()
		{
			return sampler_;
		}

		void PBRMaterial::use() const
		{
			albedo_.bindAsTexture((GLuint) TexUnits::Albedo);
			albedo_.bindAsTexture((GLuint) TexUnits::Normal);
			albedo_.bindAsTexture((GLuint) TexUnits::Roughness);
			sampler_.use((GLuint) TexUnits::Albedo);
			sampler_.use((GLuint) TexUnits::Normal);
			sampler_.use((GLuint) TexUnits::Roughness);
		}
	}
}