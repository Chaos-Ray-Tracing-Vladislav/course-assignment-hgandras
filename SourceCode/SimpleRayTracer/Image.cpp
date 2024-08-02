#include "Image.h"


Image::Image() : w(1920), h(1080){ image.insert(image.begin(), w * h, Color()); }

Image::Image(int w, int h) : w(w), h(h){ image.insert(image.begin(), w * h, Color());}

void Image::setPixel(int x, int y, Color value)
{
	assert(x < w || x>0 || y < h || y > 0);
	image[y * w + x] = value;
}

Color Image::getPixel(int x, int y){ return image.at(y * w + x); }

void Image::clear(){ image.clear(); }

void Image::writePPM(std::string name)
{
	//Write header
	std::ofstream imageFile(name, std::ios::out | std::ios::binary);
	imageFile << "P6\n";
	imageFile << w << " " << h << std::endl;
	imageFile << "255\n";

	//Write image content. I may can omit the for loop by using the vector as a buffer.
	for (int i = 0; i < w * h; i++)
	{
		Color pixel = image.at(i);
		imageFile.put((char)pixel.r);
		imageFile.put((char)pixel.g);
		imageFile.put((char)pixel.b);
	}
	imageFile.close();
}
