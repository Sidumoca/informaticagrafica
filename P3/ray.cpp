#include "ray.hpp"
#include <limits>
#include <iostream>

using namespace std;

Ray::Ray(PD origenIn, PD direccionIn) 
        : origen(origenIn), direccion(direccionIn) 
{
    if (direccionIn.isPoint()) {
        cerr << "La direccion del rayo debe ser un vector" << endl;
    }
    else if(!origenIn.isPoint()){
        cerr << "El origen del rayo debe ser un punto" << endl;
    }else{
		float mod=direccionIn.module();
		if(mod!=0){
			if(mod!=1) direccion=direccionIn/mod; //normalizar
		}else{
			cerr << "La dirección del rayo no puede ser un vecto nulo" << endl;
		}
	}
}

bool Ray::intersecta(const vector<Primitiva*>& primitivas, PD& puntoInterseccionOut, Emision& emisionOut) const{
	bool interseccionEncontrada = false;
	float distanciaMinima = numeric_limits<float>::max(); //maximo posible valor en floats
	emisionOut.r = 0;
	emisionOut.g = 0;
	emisionOut.b = 0;
	PD puntoInterseccionTemp;

	for (const auto& primitiva : primitivas) {
		if (primitiva->intersecta(*this, puntoInterseccionTemp)) {
			float distancia = (puntoInterseccionTemp - origen).module();
			if (distancia < distanciaMinima) {
				distanciaMinima = distancia;
				puntoInterseccionOut = puntoInterseccionTemp;
				emisionOut = primitiva->emision;
				interseccionEncontrada = true;
			}
		}
	}
	return interseccionEncontrada;
}

