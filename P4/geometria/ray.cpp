#include "ray.hpp"
#include <limits>
#include <random>
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

Emision BRDF(Reflectancia reflectancia){
	return reflectancia.kd / M_PI;
}

Ray calcularRayoAleatorio(const PD& puntoInterseccion, const PD& normal) {
    static thread_local std::mt19937 generator(random_device{}());

    uniform_real_distribution<float> distribution(0.0f, 1.0f);

    const float u1 = distribution(generator);
    const float u2 = distribution(generator);

    const float cosTheta = u1;
    const float sinTheta =
        sqrt(1.0f - cosTheta * cosTheta);

    const float phi =
        2.0f * static_cast<float>(M_PI) * u2;

    PD n = normal / normal.module();

    PD referencia;
    if (abs(n[0]) < 0.9f) {
        referencia = PD(1.0f, 0.0f, 0.0f, 0.0f);
    } else {
        referencia = PD(0.0f, 1.0f, 0.0f, 0.0f);
    }

    PD t = referencia.cross(n);
    t = t / t.module();

    PD b = n.cross(t);
    b = b / b.module();

    PD direccion =
          t * (sinTheta * cos(phi))
        + b * (sinTheta * sin(phi))
        + n * cosTheta;

    return Ray( puntoInterseccion + n * 0.001f, direccion);
}

bool Ray::intersecta(const Escena& escena, PD& puntoInterseccionOut, Emision& emisionOut, unsigned rebotesRestantes) const{
	if(rebotesRestantes <= 0) return false;
	
	
	bool interseccionEncontrada = false;
	float distanciaMinima = numeric_limits<float>::max(); //maximo posible valor en floats
	emisionOut.r = 0;
	emisionOut.g = 0;
	emisionOut.b = 0;
	PD puntoInterseccionTemp;
	PD normal, normalAux;
	const Primitiva* primitivaCercana = nullptr;

	//calculo la interseccion ---------------------------
	for(const auto& primitiva : escena.primitivas) {
		if(primitiva->intersecta(*this, puntoInterseccionTemp, normalAux)) {
			float distancia = (puntoInterseccionTemp - origen).module();
			if(distancia < distanciaMinima) {
				distanciaMinima = distancia;
				primitivaCercana = primitiva.get();
				puntoInterseccionOut = puntoInterseccionTemp;
				normal = normalAux;
				interseccionEncontrada = true;
			}
		}
	}

	//calculo rayos de sombra ----------------------------------
	if(interseccionEncontrada){

		if(primitivaCercana->esEmisiva){
			emisionOut = primitivaCercana->emision;
			return true;
		}

		Emision emLuz = {0,0,0};

		if(normal.dot(this->direccion) > 0) { //si esta mirando hacia el otro lado
			normal = normal * -1.0;
		}

		for(const auto& luzPuntual : escena.lucesPuntuales) {
			PD direccionLuz = luzPuntual->posicion - puntoInterseccionOut;
			float dist = direccionLuz.module();
			PD wi= direccionLuz / dist; //normalizo

			Ray rayoSombra(puntoInterseccionOut + normal * 0.001f, wi);
			bool enSombra = false;
			PD puntoInterseccionRayoSombra;
			PD auxNormal;

			for(const auto& primitiva : escena.primitivas) {
				if(primitiva->intersecta(rayoSombra, puntoInterseccionRayoSombra, auxNormal)) {
					float distObstaculo = (puntoInterseccionRayoSombra - rayoSombra.origen).module();
					if(distObstaculo < dist) {
						enSombra = true;
						break; //si hay algo antes de la luz
					}
				}
			}

			if(!enSombra){
				float coseno = max(0.0f, normal.dot(wi)); //el coseno (tercera parte)
				float reduccionDistancia = 1.0 / (dist* dist); //1/dist^2

				emLuz = emLuz + ((luzPuntual->emision * reduccionDistancia) * BRDF(primitivaCercana->reflectancia) * coseno);
			}
		}

		Ray w2 = calcularRayoAleatorio(puntoInterseccionOut, normal);

		PD puntoInterseccionAux;
		Emision luzIndirecta = {0,0,0};
		if(w2.intersecta(escena, puntoInterseccionAux, luzIndirecta, rebotesRestantes-1)){
			float coseno = max(0.0f, normal.dot(w2.direccion));
			float pdf = 1.0f / (2.0f * static_cast<float>(M_PI)); //por lo de probabilidad uniforme
			
			luzIndirecta = (luzIndirecta * coseno* BRDF(primitivaCercana->reflectancia)) / pdf;
		}

		emisionOut = emLuz + luzIndirecta;
	}
	return interseccionEncontrada;
}
