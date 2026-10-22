// En la carpeta build: "cmake ..", "cmake --build ." y "./main3"

#include <vector>
#include <iostream>
#include "esfera.hpp"
#include "monolito.hpp"
#include "plano.hpp"
#include "primitiva.hpp"
#include "ray.hpp"
#include "pd.hpp"
#include "camara.hpp"
#include "escena.hpp"
#include "luzPuntual.hpp"

using namespace std;

int main() {

	Escena escena();

	int raysPerPixel = 32;

	PD origenCamara(0,0,-3.5, 1);
	PD leftCamara(-1,0,0, 0);
	PD upCamara(0,1,0, 0);
	PD forwardCamara(0,0,3, 0);
	int width = 512, height = 512;
	
	Emision rojo= {1,0,0};
	Emision verde= {0,1,0};
	Emision azul= {0,0,1};
	Emision blanco={1,1,1};
	
	Plano leftPlane(1, PD(1,0,0,0), azul);
	Plano rightPlane(1, PD(-1,0,0,0), verde);
	Plano floorPlane(1, PD(0,1,0,0), rojo);
	Plano ceilingPlane(1, PD(0,-1,0,0), rojo);
	Plano backPlane(1, PD(0,0,-1,0), blanco);
	
	
	Emision emisionEsferaLeft = {0.8,0.6,0.9};
	Esfera leftEsfera(PD(-0.5,-0.7,0.25,1), 0.3, emisionEsferaLeft);
	Emision emisionEsferaRight = {0.5,0.9,0.9};
	Monolito rightMonolito(
		PD(0.5,-0.7,-0.25,1),
		PD(0.2,0,0,0), PD(0,0.6,0,0), PD(0,0,0.05,0),
		{0,0,0});
	
	escena.primitivas.push_back(&leftPlane);
	escena.primitivas.push_back(&rightPlane);
	escena.primitivas.push_back(&floorPlane);
	escena.primitivas.push_back(&ceilingPlane);
	escena.primitivas.push_back(&backPlane);
	escena.primitivas.push_back(&leftEsfera);
	escena.primitivas.push_back(&rightMonolito);

	// Falta añadir la luz puntual a la escena
	LuzPunto=(0,0.5,0) 

	
	// Emision em={0.2,0.3,0.9};
	// Esfera esfera(PD(0,0,0,1), 0.3, em);
	
	//escena.primitivas.push_back(&esfera);
	
	Camara camara(origenCamara, leftCamara, upCamara, forwardCamara, width, height, escena, raysPerPixel);
	camara.imprimirImagen("test.png");
}
