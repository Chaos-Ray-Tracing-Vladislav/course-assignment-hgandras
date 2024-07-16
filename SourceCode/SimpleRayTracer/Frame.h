#pragma once
#include "Vector.h"

struct Frame
{
	Matrix4 transform;

	Frame();

	Frame(const Matrix3& orientation, const Vector3& position);

	Frame(const Matrix4& transform);

	//Transforms a world point to the local frame
	Vector3 ToLocal(Vector3& p);

	Vector4 ToLocal(Vector4& p);

	//Transforms a local point to the world coordinate system
	Vector3 ToWorld(Vector3& p);

	Vector4 ToWorld(Vector4& p);

	//Generate local coordinate frame from normal in world space
	static Frame FromNormal(Vector3& n);

	Vector3 position();

	Matrix3 orientation();
};