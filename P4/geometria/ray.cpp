#include "ray.hpp"
#include <limits>
#include <random>
#include <cmath>
#include <math.h>
#include <iostream>

using namespace std;

Ray::Ray(PD origenIn, PD direccionIn) 
        : origen(origenIn), direccion(direccionIn) 
{
    if (direccionIn.isPoint()) {
        cerr << "La direccion del rayo debe ser un vector" << endl;
    }
    else if(!origenIn.isPoint()){
        cerr << "El origen del rayo debe ser un punto" << endl;
    }else{
		float mod=direccionIn.module();
		if(mod!=0){
			if(mod!=1) direccion=direccionIn/mod; //normalizar
		}else{
			cerr << "La dirección del rayo no puede ser un vecto nulo" << endl;
		}
	}
}

Emision BRDF(Reflectancia reflectancia){
	return reflectancia.kd / M_PI;
}

Ray Ray::calcularRayoAleatorio(const Escena& escena, const PD& puntoInterseccion, const PD& normal){
	static thread_local std::mt19937 generator(
        std::random_device{}()
    );

	std::uniform_real_distribution<float> distribution(0.0f, 1.0f);


	//pasar al rango correcto, con el angulo solido
    float inclinacion = acos(distribution(generator));
    float azimut = 2.0 * M_PI * distribution(generator);

	// Se usa sqrt(1 - cos^2(theta)) por la identidad sin^2(theta) + cos^2(theta) = 1,
	// evitando calcular el seno directamente; aquí debe usarse el coseno muestreado
	float senoInclinacion = sqrt(1.0 - inclinacion * inclinacion); 

	PD direccionEnLocal( senoInclinacion * cos(azimut), )



	PD u = eje / moduloEje; // u es vector unitario del eje
	PD dirCiudadEcuador = dirCiudad - (dirCiudad.dot(u))*u; // altura de la ciudad respecto al ecuador
	PD v = dirCiudadEcuador / dirCiudadEcuador.module(); // convertir en vector unitario
	PD w = u.cross(v); // producto vectorial para sacar la tercera componente

	PD x = u*cos(inclinacion)*radio;
	PD y = v*sin(inclinacion)*cos(azimut)*radio;
	PD z = w*sin(inclinacion)*sin(azimut)*radio;

	this->estacion = centro + x + y + z;
}

bool Ray::intersecta(const Escena& escena, PD& puntoInterseccionOut, Emision& emisionOut, unsigned rebotesRestantes = 5) const{
	if rebotesRestantes <= 0 return false;
	
	
	bool interseccionEncontrada = false;
	float distanciaMinima = numeric_limits<float>::max(); //maximo posible valor en floats
	emisionOut.r = 0;
	emisionOut.g = 0;
	emisionOut.b = 0;
	PD puntoInterseccionTemp;
	PD normal, normalAux;
	const Primitiva* primitivaCercana = nullptr;

	//calculo la interseccion ---------------------------
	for (const auto& primitiva : escena.primitivas) {
		if (primitiva->intersecta(*this, puntoInterseccionTemp, normalAux)) {
			float distancia = (puntoInterseccionTemp - origen).module();
			if (distancia < distanciaMinima) {
				distanciaMinima = distancia;
				primitivaCercana = primitiva.get();
				puntoInterseccionOut = puntoInterseccionTemp;
				normal = normalAux;
				interseccionEncontrada = true;
			}
		}
	}

	//calculo rayos de sombra ----------------------------------
	if(interseccionEncontrada){
		Emision emLuz = {0,0,0};

		if(normal.dot(this->direccion) > 0) { //si esta mirando hacia el otro lado
			normal = normal * -1.0;
		}

		for (const auto& luzPuntual : escena.lucesPuntuales) {
			PD direccionLuz = luzPuntual->posicion - puntoInterseccionOut;
			float dist = direccionLuz.module();
			PD wi= direccionLuz / dist; //normalizo

			Ray rayoSombra(puntoInterseccionOut + normal * 0.001f, wi);
			bool enSombra = false;
			PD puntoInterseccionRayoSombra;
			PD auxNormal;

			for (const auto& primitiva : escena.primitivas) {
				if (primitiva->intersecta(rayoSombra, puntoInterseccionRayoSombra, auxNormal)) {
					float distObstaculo = (puntoInterseccionRayoSombra - rayoSombra.origen).module();
					if (distObstaculo < dist) {
						enSombra = true;
						break; //si hay algo antes de la luz
					}
				}
			}

			if(!enSombra){
				float coseno = abs(normal.dot(wi)); //el coseno (tercera parte)
				float reduccionDistancia = 1.0 / (dist* dist); //1/dist^2

				emLuz = emLuz + ((luzPuntual->emision * reduccionDistancia) * BRDF(primitivaCercana->reflectancia) * coseno);
			}
		}

		PD w2 = calcularRayoAleatorio(escena, puntoInterseccionOut, normal);

		PD puntoInterseccionAux;
		Emision luzIndirecta = {0,0,0};
		if(w2.intersecta(escena, puntoInterseccionAux, luzIndirecta, rebotesRestantes-1)){
			float coseno = abs(normal.dot(w2));
			
			luzIndirecta = luzIndirecta * coseno* BRDF(primitivaCercana->reflectancia);
		}

		emisionOut = emLuz + luzIndirecta;
	}
	return interseccionEncontrada;
}

