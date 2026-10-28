#include "triangulo.hpp"
#include <iostream>

Triangulo::Triangulo(PD p1, PD p2, PD p3, Emision emisionIn, Reflectancia reflectanciaIn)
	:Primitiva(emisionIn, reflectanciaIn){ 
    if(!p1.isPoint() || !p2.isPoint() || !p3.isPoint()) std::cerr << "El triangulo debe estar compuesto de tres puntos." << std::endl; 
	this->p1 = p1; this->p2 = p2; this->p3 = p3;
	dir1 = p2 - p1;
	dir2 = p3 - p1;	

	normal = dir1.cross(dir2);
	normal = normal/normal.module();
	PD puntoADir(p1[0], p1[1], p2[2], 0); //TODO: pasar esto a funcion de PD
	distancia = -(normal.dot(puntoADir)); //normal*punto + dist = 0 => despejo
}

bool Triangulo::intersecta(const Ray& ray, PD& puntoInterseccionOut, PD& normalOut) const{
    if(normal.dot(ray.direccion)!=0){ //TODO: quizas cambiar a uno de estos 1e-6f con abs()
        PD origenVector(ray.origen[0], ray.origen[1], ray.origen[2], 0); // convertir origen en vector para esta operación
        float t = -((normal.dot(origenVector))+distancia)/(normal.dot(ray.direccion));
        if (t>=0){
			puntoInterseccionOut = ray.origen+(t*ray.direccion);
			
			PD aux1 = puntoInterseccionOut - p1;
			if(normal.dot(dir1.cross(aux1)) < 0) return false;

			PD edge2 = p3 - p2;
            PD aux2 = puntoInterseccionOut - p2;
            if (normal.dot(edge2.cross(aux2)) < 0) return false;

            PD aux3 = puntoInterseccionOut - p3;
            if (normal.dot(dir2.cross(aux3)) > 0) return false; //mayor que porque uso dir 2, que es invertido a lo que quiero

            normalOut = normal;
			return true;            
        }
    }
    return false;
}
