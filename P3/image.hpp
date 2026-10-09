#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <vector>

#include "OpenEXR/ImfRgbaFile.h"
#include "OpenEXR/ImfArray.h"
#include <png.h>

using namespace OPENEXR_IMF_NAMESPACE;

struct Emision {
	float r;
	float g;
	float b;

	Emision operator+(const Emision& other) const {
        return { r + other.r, g + other.g, b + other.b };
    }

	Emision operator/(float divisor) const {
		return { r / divisor, g / divisor, b / divisor };
	}
};

class Image {
public:
	Image();
	Image(unsigned int widthIN, unsigned int heightIN, bool isHDRIN);
	Image(const char *path, bool isHDRIN);
	void write(const char *path);
	void read(const char *path);
	void clamping();
	void ecualizacion();
	void ecualizacionYClamping(float V);
	void curvaGamma(float gamma);
	void curvaGammaYClamping(float V, float gamma);
	
	std::vector<std::vector<Emision>> image;
private:
	bool verificarYCambiar();
	float calcularMaximo();
	bool isHDR = false;
	unsigned int width = 0;
	unsigned int height = 0;
}; 

#endif // IMAGE_HPP