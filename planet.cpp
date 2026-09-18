#include "planet.hpp"
#include <iostream>
#include <cmath>

Planet::Planet(PD eje, PD ciudad, PD centro, float inclinacion, float azimut){
	this->eje = eje;
	this->ciudad = ciudad;
	this->centro = centro;
	this->inclinacion = inclinacion;
	this->azimut = azimut;

	dirCiudad = ciudad-centro;

	float moduloEje = eje.module();

	float radio = moduloEje/2.0;

	//verificar que radio de eje y distancia centro-ciudad es igual
	if(std::abs(radio - dirCiudad.module()) > 1e-6){
        std::cerr << "Error: radio de eje y distancia centro-ciudad no son iguales" << std::endl;
	}

	//calcular estacion

	PD u(eje[0] / moduloEje, eje[1] / moduloEje, eje[2] / moduloEje, 0.0); //TODO: hacer new?

	PD dirCiudadEcuador = dirCiudad - (dirCiudad.dot(u))*u;

	PD v = dirCiudadEcuador / dirCiudadEcuador.module();

	PD w = u.cross(v); //vectorial

	float x = radio*sin(inclinacion)*cos(azimut);
	float y = radio*sin(inclinacion)*sin(azimut);
	float z = radio*cos(inclinacion);

	this->estacion = centro + x*v + y*w + z*u;	
}