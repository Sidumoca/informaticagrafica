#ifndef CAMARA_HPP
#define CAMARA_HPP

#include "pd.hpp"
#include "image.hpp"
#include "primitiva.hpp"
#include <vector>

class Camara {
	public:
		Camara(PD origenIn, PD leftIn, PD upIn, PD forwardIn, int widthIn, int heightIn, std::vector<Primitiva*> primitivasIn);
		PD origen; 
		PD left; 
		PD up; 
		PD forward;

		int width, height;
		float anchoDePixel;
		float altoDePixel;

		PD puntoPrimerPixel;
		PD uLeft;
		PD uUp;

		std::vector<Primitiva*> primitivas;


		Emision generateRay(int i, int j);
		void imprimirImagen(const char *path);

};

#endif // CAMARA_HPP