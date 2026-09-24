#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <vector>

#include "OpenEXR/ImfRgbaFile.h"
#include "OpenEXR/ImfArray.h"
#include <png.h>

using namespace OPENEXR_IMF_NAMESPACE;

struct Pixel {
	float Rojo;
	float Verde;
	float Azul;
};

class Image {
public:
	Image();
	Image(const char *path, bool isHDRIN);
	void write(const char *path);
	void read(const char *path);
	void clamping();
private:
	std::vector<std::vector<Pixel>> image;
	bool isHDR = false;
	unsigned int width = 0;
	unsigned int height = 0;
}; 

#endif // IMAGE_HPP