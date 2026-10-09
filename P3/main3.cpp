// En la carpeta build: "cmake ..", "cmake --build ." y "./main3"

#include <vector>
#include <iostream>
#include "esfera.hpp"
#include "plano.hpp"
#include "primitiva.hpp"
#include "ray.hpp"
#include "pd.hpp"
#include "camara.hpp"

using namespace std;

vector<Primitiva*> primitivas;

int main() {
	PD origenCamara(0,0,-3.5, 1);
	PD leftCamara(-1,0,0, 0);
	PD upCamara(0,1,0, 0);
	PD forwardCamara(0,0,3, 0);
	int width = 256, height = 256;
	
	Emision rojo= {16,0,0};
	Emision verde= {0,16,0};
	Emision azul= {0,0,16};
	Emision blanco={16,16,16};
	
	Plano leftPlane(1, PD(1,0,0,0), azul);
	Plano rightPlane(1, PD(-1,0,0,0), verde);
	Plano floorPlane(1, PD(0,1,0,0), rojo);
	Plano ceilingPlane(1, PD(0,-1,0,0), rojo);
	Plano backPlane(1, PD(0,0,-1,0), blanco);
	
	
	Emision emisionEsferaLeft = {0.8,0.6,0.9};
	Esfera leftEsfera(PD(-0.5,-0.7,0.25,1), 0.3, emisionEsferaLeft);
	Emision emisionEsferaRight = {0.5,0.9,0.9};
	Esfera rightEsfera(PD(0.5,-0.7,-0.25,1), 0.3, emisionEsferaRight);
	
	primitivas.push_back(&leftPlane);
	primitivas.push_back(&rightPlane);
	primitivas.push_back(&floorPlane);
	primitivas.push_back(&ceilingPlane);
	primitivas.push_back(&backPlane);
	primitivas.push_back(&leftEsfera);
	primitivas.push_back(&rightEsfera);
	
	// Emision em={0.2,0.3,0.9};
	// Esfera esfera(PD(0,0,0,1), 0.3, em);
	
	//primitivas.push_back(&esfera);
	
	Camara camara(origenCamara, leftCamara, upCamara, forwardCamara, width, height, primitivas);
	camara.imprimirImagen("test.png");

	primitivas.clear();
}
