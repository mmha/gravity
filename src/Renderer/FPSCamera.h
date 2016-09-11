#ifndef GE_OGL4_FPSCAMERA_H
#define GE_OGL4_FPSCAMERA_H

#include "lmi/lmi.h"
#include "internal/Buffer.h"
#include "ShaderDefinitions.h"

namespace ge
{
	namespace gl4
	{
		using namespace lmi;

		class FPSCamera
		{
			public:
			FPSCamera(const vec3 &from, const vec3 &to);

			void setRotation(const mat3 &rotation);
			void setRotation(const Quat &rotation);
			void setRotation(const vec3 &rotation); //Euler Angles
			void setPosition(const vec3 &pos);
			void lookAt(const vec3 &from, const vec3 &to);

			void rotate(const mat3 &rotation);
			void rotate(const Quat &rotation);
			void rotate(const vec3 &rotation); //Euler Angles
			void move(const vec3 &pos);

			private:
			Quat quat;
			vec3 position;
		};
	}
}

#endif