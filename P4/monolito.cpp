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

	
bool Monolito::intersecta(Ray ray, PD& puntoInterseccionOut) const{
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

	for (int eje = 0; eje < 3; ++eje) {
		if (abs(direccion[eje]) < 1e-6f) {
			if (abs(origen[eje]) > 1.0f) return false; //si es paralelo y esta fuera, no hay choque
			continue;
		}

		float t1 = (-1.0f - origen[eje]) / direccion[eje];
		float t2 = ( 1.0f - origen[eje]) / direccion[eje];
		if (t1 > t2) swap(t1, t2);

		tmin = max(tmin, t1); //la entrada mas rande
		tmax = min(tmax, t2); //la salida mas pequeña
		if (tmin > tmax) return false;
	}

	if (tmax < 1e-6f) return false; //monolito detras del rayo

	//si tmin <0 el rayo sale de dentro, la salida tmax es el primer punto de contacto
	float tHit = (tmin >= 0) ? tmin : tmax;

	//no se compara con 0 por si el rayo saliera directamente de la superficie del monolito

	puntoInterseccionOut = ray.origen + ray.direccion * tHit;
	return true;
}

PD Monolito::getNormal(const PD& punto) const{
	PD aux = punto - centro;

	//proyeccion espacio local
	float cI = aux.dot(uI) / mediaLen[0];
    float cJ = aux.dot(uJ) / mediaLen[1];
    float cK = aux.dot(uK) / mediaLen[2];

	//Segun con que cara ha chocado devolvemos una normal u otra
	if (abs(cI) >= abs(cJ) && abs(cI) >= abs(cK)) {
        return (cI > 0) ? uI : (uI * -1.0f);
    } 
    else if (abs(cJ) >= abs(cI) && abs(cJ) >= abs(cK)) {
        return (cJ > 0) ? uJ : (uJ * -1.0f);
    } 
    else {
        return (cK > 0) ? uK : (uK * -1.0f);
    }
}
