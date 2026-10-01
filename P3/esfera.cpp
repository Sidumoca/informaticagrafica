
#include "esfera.hpp"
#include <iostream>

Esfera::Esfera(PD centroIn, float radioIn, Emision emisionIn)
	:Primitiva(emisionIn)
	,centro(centroIn), radio(radioIn)
{ if(!centroIn.isPoint()) std::cerr << "El centro de la esfera debe ser un punto" << std::endl; }

	
bool Esfera::intersecta(Ray ray, PD& puntoInterseccionOut) const{
	PD aux = (ray.origen - this->centro);
	float a = 1; //t^2*|d^2| //modulo de d al cuadrado, es unitario asi que 1
	float b = 2*(ray.direccion.dot(aux)) // 2td · (o-c)
	float auxModule = aux.module();
	float c = auxModule*auxModule - this->radio*this->radio; //|o-c|^2 - r^2  
	//hacer ecuacion de segundo grado y devolver la pequeña
}
