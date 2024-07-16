#include "Camera.h"

Camera::Camera()
{
	setFOV(90);
}

Camera::Camera(int w, int h, Frame frame, float FOV) :w(w), h(h), frame(frame), FOV(FOV)
{
	update_params();
}

void Camera::update_params()
{
	float alpha = FOV / 2.f;
	planeW = tanf(alpha / 180 * PI) * 2;
	planeH = planeW * h / w;
	pixelStep = planeW / w;
}

	//Casts a ray through a pixel of the image. The image plane is distance 1 from the camera
	//and the camera faces the -z direction. The returned ray is in the world coordinate system.
Ray Camera::CastRay(int x, int y)
{
	assert(x < w || x > 0 || y < h || y > 0);

	//NDC space (Not really, because these are the real world dimensions measured from the corner of the 
	// image)
	float xS = (x + 0.5f) * pixelStep;
	float yS = (y + 0.5f) * pixelStep;

	//Screen space
	xS = xS - planeW / 2;
	yS = -yS + planeH / 2;

	//Determine direction and origin
	Vector3 dir(xS, yS, -1);

	//Transform to world
	Vector3 dirPointWorld = frame.ToWorld(dir);

	return Ray(frame.position(), dirPointWorld + frame.position());
}

void Camera::setFOV(float fov)
{
	FOV = fov;
	update_params();
}

//These will all just rotate or translate the camera frame
void Camera::Dolly(float length)
{
	Matrix4 t = Matrix4::CreateTranslation(frame.orientation().col3().norm() * length);
	frame.transform = t * frame.transform;
}

void Camera::Truck(float length)
{

	Matrix4 t = Matrix4::CreateTranslation(frame.orientation().col1().norm() * length);
	frame.transform = t * frame.transform;

}

void Camera::Pedestal(float length)
{
	Matrix4 t = Matrix4::CreateTranslation(frame.orientation().col2().norm() * length);
	frame.transform = t * frame.transform;
}

void Camera::Pan(float angle)
{
	float PI = acosf(-1);
	float rad = angle / 360 * PI;
	Matrix4 rotation = Matrix4::CreateRotation(0, rad, 0);
	frame.transform = rotation * frame.transform;
}

void Camera::Tilt(float angle)
{
	float PI = acosf(-1);
	float rad = angle / 360 * PI;
	Matrix4 rotation = Matrix4::CreateRotation(rad, 0, 0);
	frame.transform = rotation * frame.transform;
}

void Camera::Roll(float angle)
{
	float PI = acosf(-1);
	float rad = angle / 360 * PI;
	Matrix4 rotation = Matrix4::CreateRotation(0, 0, rad);
	frame.transform = rotation * frame.transform;
}