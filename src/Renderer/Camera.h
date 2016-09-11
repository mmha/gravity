#ifndef GE_OGL4_FPSCAMERA_H
#define GE_OGL4_FPSCAMERA_H

#include <lmi/lmi.h>
#include "internal/Buffer.h"
#include "internal/ShaderDefinitions.h"

namespace ge
{
	namespace gl4
	{

		class FPSCamera
		{
			public:
			FPSCamera(const lmi::vec3 &from, const lmi::vec3 &to);

			//void setRotation(const lmi::mat3 &rotation);
			void setRotation(const lmi::Quat &rotation);
			void setRotation(const lmi::vec3 &rotation); //Euler Angles
			void setPosition(const lmi::vec3 &pos);
			void lookAt(const lmi::vec3 &from, const lmi::vec3 &to);

			//void rotate(const lmi::mat3 &rotation);
			void rotate(const lmi::Quat &rotation);
			void rotate(const lmi::vec3 &rotation); //Euler Angles
			void move(const lmi::vec3 &pos);
			
			void setPerspective(float fovy, float aspect, float zNear, float zFar = std::numeric_limits<float>::infinity());

			void upload();
			private:
			struct GPUData
			{
				lmi::mat4 projMat, viewMat;
			};

			lmi::Quat quat;
			lmi::vec3 position;
			lmi::mat4 projMat;
			Buffer buffer;
		};
	}
}

#endif