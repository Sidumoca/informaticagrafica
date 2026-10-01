
#include "plano.hpp"
#include <iostream>

Plano::Plano(float distanciaIn, PD normalIn, Emision emisionIn)
	:Primitiva(emisionIn)
	,distancia(distanciaIn), normal(normalIn){ 
    if(normalIn.isPoint()) std::cerr << "La normal del plano debe ser una dirección." << std::endl; 
}

bool Plano::intersecta(Ray ray, PD& puntoInterseccionOut) const{
    bool hay_interseccion=false;
    if(normal.dot(ray.direccion)!=0){
        hay_interseccion=true
        float t = (normal*ray.origen+distancia)/(normal*ray.direccion);
        puntoInterseccionOut=o+t*ray.direccion;
    }
    return hay_interseccion;
}
