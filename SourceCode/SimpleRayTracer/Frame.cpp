#pragma once
#include "Frame.h"


Frame::Frame() : transform(Matrix4::Identity()){ }

Frame::Frame(const Matrix3& orientation, const Vector3& position) : transform(Matrix4(orientation, position)){ }

Frame::Frame(const Matrix4& transform) : transform(transform){ }

//Transforms a world point to the local frame
Vector3 Frame::ToLocal(Vector3& p)
{
	Vector4 point(p);
	Vector4 transformed = ToLocal(point);
	return Vector3(transformed.x, transformed.y, transformed.z);
}

Vector4 Frame::ToLocal(Vector4& p)
{
	return transform * p;
}

//Transforms a local point to the world coordinate system
Vector3 Frame::ToWorld(Vector3& p)
{
	Vector4 point(p);
	Vector4 transformed = ToWorld(point);
	return transformed.xyz();
}

Vector4 Frame::ToWorld(Vector4& p)
{
	//Invert rotation
	Vector4 c1 = transform.col1();
	Vector4 c2 = transform.col2();
	Vector4 c3 = transform.col3();

	Matrix3 rot(c1.xyz(), c2.xyz(), c3.xyz());

	Matrix4 inverse(rot, -transform.col4().xyz());
	return inverse * p;
}

//Generate local coordinate frame from normal in world space
Frame Frame::FromNormal(Vector3& n)
{
	Vector3 up = Vector3::up();
	Vector3 z = Cross(n, up);
	Vector3 y = Cross(n, z);
	Vector3 x = Cross(y, z);

	Matrix3 rot(x, y, z);
	Matrix4 t(rot);
	return Frame(t);
}

Vector3 Frame::position(){ return transform.col4().xyz(); }

Matrix3 Frame::orientation()
{ 
	return Matrix3(transform.col1().xyz(), transform.col2().xyz(), transform.col3().xyz());
}
