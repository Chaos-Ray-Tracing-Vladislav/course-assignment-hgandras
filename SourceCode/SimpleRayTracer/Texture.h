#pragma once
#include <string>
#include "Vector.h"
#include "stb_image.h"

enum TextureType
{
	ALBEDO,
	EDGES,
	CHECKER,
	BITMAP
};

struct Texture {
	std::string name;
	TextureType type;
	Texture(std::string,TextureType);
	virtual Vector3 Sample(Vector2, Vector3);
};

struct ColorTexture : Texture {
	Vector3 albedo;
	Vector3 Sample(Vector2, Vector3);
	ColorTexture(std::string,TextureType,Vector3);
};

struct EdgeTexture : Texture {
	Vector3 edgeColor;
	Vector3 innerColor;
	EdgeTexture(std::string,TextureType,Vector3,Vector3,float);
	float edgeWidth;
	Vector3 Sample(Vector2, Vector3);
	
};

struct CheckerTexture : Texture {
	Vector3 ColorA;
	Vector3 ColorB;
	float squareSize;
	Vector3 Sample(Vector2, Vector3);
	CheckerTexture(std::string, TextureType, Vector3, Vector3, float);
};

struct BitmapTexture : Texture {
	int w = 0;
	int h = 0;
	int c = 0;
	unsigned char* image;
	Vector3 Sample(Vector2, Vector3);
	BitmapTexture(std::string, TextureType);
	BitmapTexture(std::string, TextureType, int, int, int, unsigned char*);
};

