#ifndef GE_SAMPLER_H
#define GE_SAMPLER_H
#include <glbinding/gl45/gl.h>

namespace ge
{
	namespace gl4
	{
		using namespace gl45;
		
		class Sampler
		{
		public:
			Sampler();
			Sampler(const Sampler &) = delete;
			Sampler(Sampler &&);
			Sampler &operator=(Sampler &&other);
			~Sampler();

			void set(GLenum parameterName, GLfloat value);
			void set(GLenum parameterName, GLint value);
			void set(GLenum parameterName, GLenum value);
			void set(GLenum parameterName, const GLfloat *values);
			void set(GLenum parameterName, const GLint *values);
			void set(GLenum parameterName, const GLuint *values);

			void use(GLuint textureUnit) const;
			GLuint id() const;
		private:
			GLuint samplerID_;
		};
	}
}

#endif //OUTWEST_SAMPLER_H
