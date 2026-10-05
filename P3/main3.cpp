// En la carpeta build: "cmake ..", "cmake --build ." y "./main3"

#include <vector>
#include <iostream>
#include "esfera.hpp"
#include "plano.hpp"
#include "primitiva.hpp"
#include "ray.hpp"
#include "pd.hpp"

using namespace std;

vector<Primitiva*> primitivas;


int main() {
	PD centroEsfera(2,0,0,1);
	Emision emisionEsfera = {10,20,30};
	Esfera esfera(centroEsfera, 1.0, emisionEsfera);

	PD normalPlano(0,1,0,0);
	Emision emisionPlano = {30,20,10};
	Plano plano(-2.0, normalPlano, emisionPlano);

	primitivas.push_back(&esfera);
	primitivas.push_back(&plano);

	// No intersección
	//PD origenRayo(0,0,0,1);
	//PD direccionRayo(0,-1,0,0);

	// Intersección con plano: (0,2,0)
	//PD origenRayo(0,0,0,1);
	//PD direccionRayo(0,1,0,0);

	// Intersección con esfera: (1,0,0)
	//PD origenRayo(0,0,0,1);
	//PD direccionRayo(1,0,0,0);

	// Intersección ambos: (1.02, 0.20, 0)
	PD origenRayo(0,0,0,1);
	PD direccionRayo(1,0.2,0,0);

	Ray rayo(origenRayo, direccionRayo);
	PD puntoInterseccion;
	if (rayo.intersecta(primitivas, puntoInterseccion)) {
		cout << "Interseccion encontrada en: " << puntoInterseccion[0] << ", " << puntoInterseccion[1] << ", " << puntoInterseccion[2] << endl;
	} else {
		cout << "No hay interseccion con las primitivas." << endl;
	}

	primitivas.clear();
}
