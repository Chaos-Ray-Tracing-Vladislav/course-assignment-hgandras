#pragma once
#include "Vector.h"
#include "Frame.h"
#include "Image.h"
#include "Ray.h"

const double PI = acos(-1);

//TODO: Maybe just passing the image dimensions is enough instead of the whole image
class Camera {
private:
	float planeW;
	float planeH;
	float pixelStep;

	void update_params();

public:
	Frame frame;
	float FOV;
	int w, h;

	Camera();

	Camera(int w, int h, Frame frame, float FOV);

	//Casts a ray through a pixel of the image. The image plane is distance 1 from the camera
	//and the camera faces the -z direction. The returned ray is in the world coordinate system.
	Ray CastRay(int x, int y);

	void setFOV(float fov);

	//These will all just rotate or translate the camera frame
	void Dolly(float length);

	void Truck(float length);

	void Pedestal(float length);

	void Pan(float angle);

	void Tilt(float angle);

	void Roll(float angle);
};
