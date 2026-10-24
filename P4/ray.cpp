#include "ray.hpp"
#include <limits>
#include <cmath>
#include <math.h>
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
	Primitiva* primitivaCercana;

	for (const auto& primitiva : escena.primitivas) {
		if (primitiva->intersecta(*this, puntoInterseccionTemp)) {
			float distancia = (puntoInterseccionTemp - origen).module();
			if (distancia < distanciaMinima) {
				distanciaMinima = distancia;
				primitivaCercana = primitiva;
				puntoInterseccionOut = puntoInterseccionTemp;
				interseccionEncontrada = true;
			}
		}
	}

	if(interseccionEncontrada){
		Emision emLuz = {0,0,0};

		PD normal = primitivaCercana -> getNormal(puntoInterseccionOut);
		if(normal.dot(this->direccion) > 0) { //si esta mirando hacia el otro lado
			normal = normal * -1.0;
		}

		for (const auto& luzPuntual : escena.lucesPuntuales) {
			PD direccionLuz = luzPuntual->posicion - puntoInterseccionOut;
			float dist = direccionLuz.module();
			PD wi= direccionLuz / dist; //normalizo

			Ray rayoSombra(puntoInterseccionOut + normal * 0.00001f, wi);
			bool enSombra = false;
			PD puntoInterseccionRayoSombra;

			for (const auto& primitiva : escena.primitivas) {
				if (primitiva->intersecta(rayoSombra, puntoInterseccionRayoSombra)) {
					float distObstaculo = (puntoInterseccionRayoSombra - rayoSombra.origen).module();
					if (distObstaculo < dist) {
						enSombra = true;
						break; //si hay algo antes de la luz
					}
				}
			}

			if(!enSombra){
				float coseno = abs(normal.dot(wi)); //el coseno (tercera parte)
				float reduccionDistancia = 1.0 / (dist* dist); //1/dist^2

				emLuz = emLuz + ((luzPuntual->emision * reduccionDistancia) * (primitivaCercana->emision / M_PI) * coseno);
			}
		}
		emisionOut = emLuz;
	}
	return interseccionEncontrada;
}

