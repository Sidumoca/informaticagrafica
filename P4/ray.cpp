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

bool Ray::intersecta(Escena escena, PD& puntoInterseccionOut, Emision& emisionOut) const{
	bool interseccionEncontrada = false;
	float distanciaMinima = numeric_limits<float>::max(); //maximo posible valor en floats
	emisionOut.r = 0;
	emisionOut.g = 0;
	emisionOut.b = 0;
	PD puntoInterseccionTemp;
	Primitiva primitivaAux;

	for (const auto& primitiva : escena.primitivas) {
		if (primitiva->intersecta(*this, puntoInterseccionTemp)) {
			float distancia = (puntoInterseccionTemp - origen).module();
			if (distancia < distanciaMinima) {
				distanciaMinima = distancia;
				primitivaAux.copy(primitiva);
				puntoInterseccionOut = puntoInterseccionTemp;
				interseccionEncontrada = true;
			}
		}
	}

	if(interseccionEncontrada){
		float Emision = {0,0,0};
		for (const auto& luzPuntual : escena.lucesPuntuales) {
			PD direccionLuz = luzPuntual->posicion - puntoInterseccionOut;
			Ray rayoHaciaLuz(puntoInterseccionOut, direccionLuz);
			PD puntoInterseccionLuz;
			Emision emisionLuz;
			if (rayoHaciaLuz.intersecta(escena, puntoInterseccionLuz, emisionLuz)) {
				PD dirAux = puntoInterseccionLuz-puntoInterseccionOut;
				if(dirAux.module() < direccionLuz.module()){ //si hay algo antes de la luz
					emisionOut = primitivaAux.emision;
					return true;
				}
			}
			//si no hay nada antes de la luz (no intersecta o intersecta despues)
			Emision += primitivaAux.emision * luzPuntual->emision;
		}
	}
	return interseccionEncontrada;
}

