#include "Texture.h"

Texture::Texture(std::string name,TextureType type) : name(name),type(type) {}
Vector3 Texture::Sample(Vector2 uv, Vector3 bary) { return Vector3::zero(); }

ColorTexture::ColorTexture(std::string name, TextureType type, Vector3 albedo) : Texture(name, type), albedo(albedo) { }

Vector3 ColorTexture::Sample(Vector2 uv,Vector3 bary)
{
	return albedo;
}

EdgeTexture::EdgeTexture(std::string name, TextureType type, Vector3 edgeColor, Vector3 innerColor, float edgeWidth) : Texture(name, type), edgeColor(edgeColor), innerColor(innerColor), edgeWidth(edgeWidth) {}

Vector3 EdgeTexture::Sample(Vector2 uv,Vector3 bary)
{
	if (bary.x < edgeWidth || bary.y < edgeWidth || 1-bary.x-bary.y < edgeWidth)
		return edgeColor;
	return innerColor;
}

CheckerTexture::CheckerTexture(std::string name, TextureType type, Vector3 colA, Vector3 colB, float square_size) : Texture(name, type), ColorA(colA), ColorB(colB), squareSize(square_size) {}

Vector3 CheckerTexture::Sample(Vector2 uv,Vector3 bary)
{
	int x = floor(uv.x / squareSize);
	int y = floor(uv.y / squareSize);

	int remainderX = x % 2;
	int remainderY = y % 2;

	if ((remainderX + remainderY) % 2 == 0)
		return ColorA;

	return ColorB;
}

BitmapTexture::BitmapTexture(std::string name, TextureType type) : Texture(name, type) {};
BitmapTexture::BitmapTexture(std::string mame, TextureType type, int w, int h, int c, unsigned char* image) : Texture(name, type), w(w),h(h),c(c),image(image){}

Vector3 BitmapTexture::Sample(Vector2 uv,Vector3 bary)
{
	int x = floor(uv.x * w);
	int y = floor((1-uv.y) * h);
	int offset = (y * w + x) * c;
	unsigned char* pixel = image + offset;
	
	return Vector3(*pixel/255.f, *(pixel + 1)/255.f, *(pixel + 2)/255.f);
}