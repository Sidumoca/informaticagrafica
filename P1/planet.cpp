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

	//calcular posición estacion

	PD u = eje / moduloEje; // u es vector unitario del eje
	PD dirCiudadEcuador = dirCiudad - (dirCiudad.dot(u))*u; // altura de la ciudad respecto al ecuador
	PD v = dirCiudadEcuador / dirCiudadEcuador.module(); // convertir en vector unitario
	PD w = u.cross(v); // producto vectorial para sacar la tercera componente

	PD x = u*cos(inclinacion)*radio;
	PD y = v*sin(inclinacion)*cos(azimut)*radio;
	PD z = w*sin(inclinacion)*sin(azimut)*radio;

	this->estacion = centro + x + y + z;


	//normal de la superficie
	normalEstacion = estacion-centro;
	normalEstacion = normalEstacion / normalEstacion.module(); //para que modulo sea 1

	//tangente a la longitud
	tangenteLongitud = u.cross(normalEstacion); //uso u que es eje normalizado
	tangenteLongitud = tangenteLongitud / tangenteLongitud.module();
	//tangente a la latitud
	tangenteLatitud = normalEstacion.cross(tangenteLongitud);
	tangenteLatitud = tangenteLatitud / tangenteLatitud.module();

}