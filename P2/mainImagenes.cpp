#include "image.hpp"

int main(){
	Image imClamping("../../PrismsLenses.exr", true);
	imClamping.clamping();
	imClamping.write("test_clamping.png");

	Image imEcualizacion("../../PrismsLenses.exr", true);
	imEcualizacion.ecualizacion();
	imEcualizacion.write("test_ecualizacion.png");

	Image imCurvaGamma("../../PrismsLenses.exr", true);
	imCurvaGamma.curvaGamma(2.2f);
	imCurvaGamma.write("test_curva_gamma.png");

	Image imCurvaGammaYClamping("../../PrismsLenses.exr", true);
	imCurvaGammaYClamping.curvaGammaYClamping(1.0f, 2.2f);
	imCurvaGammaYClamping.write("test_curva_gamma_y_clamping.png");
}