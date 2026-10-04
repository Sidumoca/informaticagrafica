
#include "esfera.hpp"
#include <cmath>
#include <iostream>

Esfera::Esfera(PD centroIn, float radioIn, Emision emisionIn)
	:Primitiva(emisionIn)
	,centro(centroIn), radio(radioIn)
{ if(!centroIn.isPoint()) std::cerr << "El centro de la esfera debe ser un punto" << std::endl; }

	
bool Esfera::intersecta(Ray ray, PD& puntoInterseccionOut) const{
	PD aux = (ray.origen - this->centro);
	float a = 1; //t^2*|d^2| //modulo de d al cuadrado, es unitario asi que 1
	float b = 2*(ray.direccion.dot(aux)); // 2td · (o-c)
	float auxModule = aux.module();
	float c = auxModule*auxModule - this->radio*this->radio; //|o-c|^2 - r^2  
	float discriminante = b*b - 4*a*c;
	if (discriminante < 0) return false;

	float raiz = std::sqrt(discriminante);
	float t1 = (-b - raiz) / (2*a);
	float t2 = (-b + raiz) / (2*a);
	float t = (t1 >= 0) ? t1 : t2;
	if (t < 0) return false; //para atras no los cuento

	puntoInterseccionOut = ray.origen + ray.direccion * t;
	return true;
	
}
