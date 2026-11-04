#include "ray.hpp"
#include <limits>
#include <random>
#include <cmath>
#include <iostream>

using namespace std;

enum suceso { DIFUSO, ESPECULAR, TRANSMISION, MUERTE };

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

Emision BRDF(Reflectancia reflectancia, suceso suceso){
	if(suceso==DIFUSO){
		return reflectancia.kd; //es precioso y se simplifica todo
	} else if(suceso==ESPECULAR){
		return reflectancia.ks; //coseno y delta se cancelan 
	} else if(suceso==TRANSMISION){
		return reflectancia.kt; //coseno y delta se cancelan
	} else{
		return {0,0,0};
	}
}

Ray calcularRayoAleatorio(const PD& puntoInterseccion, const PD& normal){
    static thread_local std::mt19937 generator(random_device{}());

    uniform_real_distribution<float> distribution(0.0f, 1.0f);

    const float u1 = distribution(generator);
    const float u2 = distribution(generator);

    const float cosTheta = sqrt(1.0f - u1);
    const float sinTheta = sqrt(u1);

    const float phi =
        2.0f * static_cast<float>(M_PI) * u2;

    PD n = normal / normal.module();

    PD referencia;
    if (abs(n[0]) < 0.9f) {
        referencia = PD(1.0f, 0.0f, 0.0f, 0.0f);
    } else {
        referencia = PD(0.0f, 1.0f, 0.0f, 0.0f);
    }

    PD t = referencia.cross(n);
    t = t / t.module();

    PD b = n.cross(t);
    b = b / b.module();

    PD direccion =
          t * (sinTheta * cos(phi))
        + b * (sinTheta * sin(phi))
        + n * cosTheta;

    return Ray( puntoInterseccion + n * 0.001f, direccion);
}

bool Ray::intersecta(const Escena& escena, PD& puntoInterseccionOut, Emision& emisionOut) const{
	bool interseccionEncontrada = false;
	float distanciaMinima = numeric_limits<float>::max(); //maximo posible valor en floats
	emisionOut.r = 0;
	emisionOut.g = 0;
	emisionOut.b = 0;
	PD puntoInterseccionTemp;
	PD normal, normalAux;
	const Primitiva* primitivaCercana = nullptr;

	//calculo la interseccion ---------------------------
	for(const auto& primitiva : escena.primitivas) {
		if(primitiva->intersecta(*this, puntoInterseccionTemp, normalAux)) {
			float distancia = (puntoInterseccionTemp - origen).module();
			if(distancia < distanciaMinima) {
				distanciaMinima = distancia;
				primitivaCercana = primitiva.get();
				puntoInterseccionOut = puntoInterseccionTemp;
				normal = normalAux;
				interseccionEncontrada = true;
			}
		}
	}



	if(interseccionEncontrada){
		if(primitivaCercana->esEmisiva){
			emisionOut = primitivaCercana->emision;
			return true;
		}

		bool entrando = normal.dot(this->direccion) < 0.0f; //si esta mirando hacia el otro lado
		normal = entrando ? normal : normal * -1.0f;
		
		//calcular luce indirecta -----------------------------
		Ray w2(puntoInterseccionOut, this->direccion);//valores cualquiera temporales
		suceso suceso;
		float probabilidadSuceso;

		//ruleta rusa :)
		static thread_local std::mt19937 generator(std::random_device{}());
		std::uniform_real_distribution<float> dist(0.0f, 1.0f);
		float x = dist(generator);

		float probabilidadNoMuerte = (0.9f);
		float kdPorcentaje = primitivaCercana->reflectancia.kdPorcentaje * probabilidadNoMuerte;
		float ksPorcentaje = primitivaCercana->reflectancia.ksPorcentaje * probabilidadNoMuerte;
		float ktPorcentaje = primitivaCercana->reflectancia.ktPorcentaje * probabilidadNoMuerte;

		if(x < kdPorcentaje){
			//rayo aleatorio hacia fuera (rebote)
			suceso = DIFUSO;
			probabilidadSuceso = kdPorcentaje;
			w2 = calcularRayoAleatorio(puntoInterseccionOut, normal);

		}else if(x < kdPorcentaje + ksPorcentaje){
			//reflectancia especular perfecta
			PD direccionReflectada = this->direccion - normal * 2.0f * normal.dot(this->direccion); //como mi rayo entra y no sale invierto la resta
			suceso = ESPECULAR;
			probabilidadSuceso = ksPorcentaje;
			w2 = Ray(puntoInterseccionOut + normal * 0.001f, direccionReflectada);

		}else if(x < kdPorcentaje + ksPorcentaje + ktPorcentaje){
			//refraccion perfecta con ley de Snell
			float indiceRefraccion = 1.5f; //TODO: quizas añadir a primitiva
			float indiceRefraccionCorrecto = entrando ? (1.0f / indiceRefraccion) : indiceRefraccion; 
			float cosi = -this->direccion.dot(normal);
			float k = 1.0f - indiceRefraccionCorrecto * indiceRefraccionCorrecto * (1.0f - cosi * cosi);

			if (k < 0.0f) {
				//si no puede refractar, es un espejo
				PD direccionReflejada = this->direccion - normal * (2.0f * this->direccion.dot(normal));
				w2 = Ray(puntoInterseccionOut + normal * 0.001f, direccionReflejada);
			} else {
				PD dirRefractada = this->direccion * indiceRefraccionCorrecto + normal * (indiceRefraccionCorrecto * cosi - sqrt(k));
				//offset hacia dentro de la superficie (-normal) para atravesarla
				w2 = Ray(puntoInterseccionOut - normal * 0.001f, dirRefractada);
			}
			suceso = TRANSMISION;
			probabilidadSuceso = ktPorcentaje;
		}else{
			suceso = MUERTE;
		}
		
		Emision luzIndirecta = {0,0,0};
		if(suceso != MUERTE){
			PD puntoInterseccionAux;
			if(w2.intersecta(escena, puntoInterseccionAux, luzIndirecta)){
				//float coseno = max(0.0f, normal.dot(w2.direccion));
				
				luzIndirecta = (luzIndirecta * BRDF(primitivaCercana->reflectancia, suceso)) / probabilidadSuceso;
			}
		}
		
		
		//calculo rayos de sombra ----------------------------------
		Emision emLuz = {0,0,0};

		for(const auto& luzPuntual : escena.lucesPuntuales) {
			PD direccionLuz = luzPuntual->posicion - puntoInterseccionOut;
			float dist = direccionLuz.module();
			PD wi= direccionLuz / dist; //normalizo

			Ray rayoSombra(puntoInterseccionOut + normal * 0.001f, wi);
			bool enSombra = false;
			PD puntoInterseccionRayoSombra;
			PD auxNormal;

			for(const auto& primitiva : escena.primitivas) {
				if(primitiva->intersecta(rayoSombra, puntoInterseccionRayoSombra, auxNormal)) {
					float distObstaculo = (puntoInterseccionRayoSombra - rayoSombra.origen).module();
					if(distObstaculo < dist) {
						enSombra = true;
						break; //si hay algo antes de la luz
					}
				}
			}

			if(!enSombra){
				float coseno = max(0.0f, normal.dot(wi)); //el coseno (tercera parte)
				float reduccionDistancia = 1.0 / (dist* dist); //1/dist^2

				emLuz = emLuz + ((luzPuntual->emision * reduccionDistancia) * (primitivaCercana->reflectancia.kd / M_PI) * coseno);
			}
		}

		emisionOut = emLuz + luzIndirecta;
	}
	return interseccionEncontrada;
}
