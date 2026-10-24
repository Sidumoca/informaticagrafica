#include "plano.hpp"
#include <iostream>

Plano::Plano(float distanciaIn, PD normalIn, Emision emisionIn, Reflectancia reflectanciaIn)
	:Primitiva(emisionIn, reflectanciaIn)
	,distancia(distanciaIn){ 
    if(normalIn.isPoint()) std::cerr << "La normal del plano debe ser una dirección." << std::endl; 
    //normalizo normal porsiaca
    this->normal = normalIn/normalIn.module();
}

bool Plano::intersecta(Ray ray, PD& puntoInterseccionOut) const{
    bool hay_interseccion=false;
    if(normal.dot(ray.direccion)!=0){
        PD origenVector(ray.origen[0], ray.origen[1], ray.origen[2], 0); // convertir origen en vector para esta operación
        float t = -((normal.dot(origenVector))+distancia)/(normal.dot(ray.direccion));
        if (t>=0){
            puntoInterseccionOut=ray.origen+(t*ray.direccion);
            hay_interseccion=true;
        }
    }
    return hay_interseccion;
}

PD Plano::getNormal(const PD& punto) const{
    return this->normal;
}
