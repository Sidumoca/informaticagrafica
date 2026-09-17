#include "planet.hpp"
#include <iostream>

Planet::Planet(PD eje, PD ciudad, PD centro, float inclinacion, float azimut){
	this->eje = eje;
	this->ciudad = ciudad;
	this->centro = centro;
	this->inclinacion = inclinacion;
	this->azimut = azimut;

	dirCiudad = ciudad-centro;

	float radio = module(eje);

	//verificar que radio de eje y distancia centro-ciudad es igual
	if(radio/2 != module(dirCiudad)){
        STD::cerr << "Error: radio de eje y distancia centro-ciudad no son iguales" << std::endl;
		return 1;
	}

	//calcular estacion
	// float x = centro[0] + radio*sin(azimut)*cos(inclinacion);
	// float y = centro[1] + radio*sin(azimut)*sin(inclinacion);
	// float z = centro[2] + radio*cos(azimut);
	// this->estacion = new PD(x,y,z,true)

	
	
}