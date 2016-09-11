#include "Primitive.h"
#include "PBRMaterial.h"
#include "GeometryData.h"

namespace ge
{
	namespace gl4
	{
		Primitive::Primitive(GeometryData &geometryData, PBRMaterial &material)
		: geometryData_(geometryData)
		, material_(material)
		{

		}

		GeometryData &Primitive::geometryData()
		{
			return geometryData_;
		}

		PBRMaterial &Primitive::material()
		{
			return material_;
		}

		void Primitive::draw() const
		{
			material_.use();
			geometryData_.draw();
		}
	}
}