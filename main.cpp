#include <iostream>
#include <cmath>
#include "planet.hpp"

//TODO: usar shared_ptr

bool calcularTrayectoriaUnidireccional(Planet A, Planet B){
	PD dirLanzamiento = B.estacion - A.estacion;
	dirLanzamiento = dirLanzamiento / dirLanzamiento.module();

	//por si hay que pasar a coordenadas locales del planeta A
	float x = dirLanzamiento.dot(A.tangenteLongitud);
	float y = dirLanzamiento.dot(A.tangenteLatitud);
	float z = dirLanzamiento.dot(A.normalEstacion);

	// cos(x) = dirLanzamiento*TangenteLongitud
	float angulo1 = acos(x);
    // cos(y) = dirLanzamiento*TangenteLatitud
	float angulo2 = acos(y);
    // x < 180 && y < 180
	if(angulo1 > 180)...

    return true;
}

bool calcularTrayectoria(Planet A, Planet B){
    bool ok = calcularTrayectoriaUnidireccional(A,B);
	if(!ok){
		std::cerr << "Colision con el planeta de origen" << std::endl;
	}
	ok = calcularTrayectoriaUnidireccional(B,A);
	if(!ok){
		std::cerr << "Colision con el planeta de destino" << std::endl;
	}
	return ok;
}