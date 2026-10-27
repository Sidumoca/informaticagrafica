#include "monolito.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>

using namespace std;

Monolito::Monolito(PD c, PD i, PD j, PD k, Emision emisionIn, Reflectancia reflectanciaIn)
	:Primitiva(emisionIn, reflectanciaIn)
	,centro(c), i(i), j(j), k(k)
{ if(!centro.isPoint()) cerr << "El centro del monolito debe ser un punto" << endl; 
  if(i.isPoint() || j.isPoint() || k.isPoint()) cerr << "Los vectores i, j y k del monolito deben ser vectores" << endl;

	//longitd entre 2 porque se mira de -1 a 1
	this->mediaLen[0] = i.module()/2.0f;
	this->mediaLen[1] = j.module()/2.0f;
	this->mediaLen[2] = k.module()/2.0f;

	this->uI = i/i.module();
	this->uJ = j/j.module();
	this->uK = k/k.module();
}

	
bool Monolito::intersecta(const Ray& ray, PD& puntoInterseccionOut, PD& normalOut) const{
	PD aux = ray.origen - centro;
	//TODO: quizas añadir soporte a todo tipo de paralelepipedo y no solo rectangulo, o al menos comprobar que i,j,k son paralelos
	const float origen[3] = {
		aux.dot(uI) / mediaLen[0],
		aux.dot(uJ) / mediaLen[1],
		aux.dot(uK) / mediaLen[2]
	};
	const float direccion[3] = {
		ray.direccion.dot(uI) / mediaLen[0],
		ray.direccion.dot(uJ) / mediaLen[1],
		ray.direccion.dot(uK) / mediaLen[2]
	};

	float tmin = -numeric_limits<float>::infinity();
	float tmax = numeric_limits<float>::infinity();

	int ejeMin = 0;
    float signoMin = 0.0f;
    int ejeMax = 0;
    float signoMax = 0.0f;

	for (int eje = 0; eje < 3; ++eje) {
		if (abs(direccion[eje]) < 1e-6f) {
			if (abs(origen[eje]) > 1.0f) return false; //si es paralelo y esta fuera, no hay choque
			continue;
		}

		float t1 = (-1.0f - origen[eje]) / direccion[eje];
		float t2 = ( 1.0f - origen[eje]) / direccion[eje];
		float s1 = -1.0f; //normal hacia -u
		float s2 = 1.0f; //normal hacia u

		if (t1 > t2) {
			swap(t1, t2);
			swap(s1, s2);
		}

		if(t1 > tmin) {
			tmin = t1;
			ejeMin = eje;
			signoMin = s1;
		}
		if(t2 < tmax){
			tmax = t2;
			ejeMax = eje;
			signoMax = s2;
		}

		if (tmin > tmax) return false;
	}

	if (tmax < 1e-6f) return false; //monolito detras del rayo

	float tHit;
    int ejeHit;
    float signoHit;

    if (tmin >= 0) { //(al final si lo hago, esta solucionado en ray.cpp para todos) no comparo con 0 para que no se choque consigo mismo un rayo que salga de la superficie
        //impacto exterior normal
        tHit = tmin;
        ejeHit = ejeMin;
        signoHit = signoMin;
    } else {
        //si sale de dentro
        tHit = tmax;
        ejeHit = ejeMax;
        signoHit = signoMax;
    }

	puntoInterseccionOut = ray.origen + ray.direccion * tHit;
	const PD ejes[3] = { uI, uJ, uK };
	normalOut = ejes[ejeHit] * signoHit;
	return true;
}
