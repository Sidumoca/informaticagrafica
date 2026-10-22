#ifndef CAMARA_HPP
#define CAMARA_HPP

#include "pd.hpp"
#include "image.hpp"
#include "escena.hpp"
#include <vector>

class Camara {
	public:
		Camara(PD origenIn, PD leftIn, PD upIn, PD forwardIn, int widthIn, int heightIn, Escena escenaIn,  int raysPerPixelIn);
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

		Escena escena;
		int raysPerPixel;

		Emision generateRay(int i, int j, float multiplierX, float multiplierY);
		Emision generateKRays(int i, int j, int k);
		void imprimirImagen(const char *path);

};

#endif // CAMARA_HPP