#include "Camera.h"

using namespace ge;
using namespace ge::gl4;

FPSCamera::FPSCamera(const lmi::vec3 &from, const lmi::vec3 &to)
	: quat(1, 0, 0, 0)
	, position(0)
	, buffer(2 * sizeof(GPUData))
{
	buffer.bindToIndex(GL_UNIFORM_BUFFER, (uint32_t)UniformLocations::GE_ULOCATION_CAMERA);
	lookAt(from, to);
	upload();
}

void FPSCamera::setRotation(const lmi::Quat &rotation)
{
	quat = rotation;
}

void FPSCamera::setRotation(const lmi::vec3 &rotation)	// Euler Angles
{
	auto X = lmi::createRotationQuaternion({1, 0, 0}, rotation.x);
	auto Y = lmi::createRotationQuaternion({0, 1, 0}, rotation.y);
	auto Z = lmi::createRotationQuaternion({0, 0, 1}, rotation.z);
	quat   = Z * X * Y;
}

void FPSCamera::setPosition(const lmi::vec3 &pos)
{
	position = pos;
}

void FPSCamera::lookAt(const lmi::vec3 &from, const lmi::vec3 &to)
{
	position   = from;
	auto look  = lmi::normalize(to - from);
	auto up	   = lmi::normalize(lmi::perpendicular(lmi::vec3(0, 1, 0), look));
	auto right = lmi::cross(up, look);

	// FIXME Dat math
	/*
	auto viewMat = lmi::mat4(	right.x, up.x, look.x, 0,
								right.y, up.y, look.y, 0,
								right.z, up.z, look.z, 0,
								0,		 0,	   0,	   1);
	quat		 = lmi::Quat(viewMat);*/
}

void FPSCamera::rotate(const lmi::Quat &rotation)
{
	quat *= rotation;
}

void FPSCamera::rotate(const lmi::vec3 &rotation)	// Euler Angles
{
	auto X = lmi::createRotationQuaternion({1, 0, 0}, rotation.x);
	auto Y = lmi::createRotationQuaternion({0, 1, 0}, rotation.y);
	auto Z = lmi::createRotationQuaternion({0, 0, 1}, rotation.z);
	quat   = Z * X * quat * Y;
}

void FPSCamera::move(const lmi::vec3 &pos)
{
	auto conj			 = lmi::conjugate(quat);
	auto rotatedMovement = lmi::rotate(conj, pos);
	position += rotatedMovement;
}

void FPSCamera::setPerspective(float fovy, float aspect, float zNear, float zFar)
{
	projMat = lmi::perspective(fovy, aspect, zNear, zFar);
}

void FPSCamera::upload()
{
	lmi::mat4 viewMat = static_cast<lmi::mat4>(quat);
	auto translation  = lmi::translate(position.x, position.y, position.z);
	viewMat			  = viewMat * translation;

	buffer.upload(0, sizeof(lmi::mat4), projMat);
	buffer.upload(sizeof(lmi::mat4), sizeof(lmi::mat4), viewMat);
}
