#ifndef PRIMITIVA_HPP
#define PRIMITIVA_HPP

#include "../geometria/pd.hpp"
#include "../image.hpp"
#include <algorithm>
#include <iostream>

class Ray; // Declaración adelantada porque sino hay dependencia circular.

struct Reflectancia {
    Emision kd{0.0f, 0.0f, 0.0f};
    float kdPorcentaje{0.0f};
    Emision ks{0.0f, 0.0f, 0.0f};
    float ksPorcentaje{0.0f};
    Emision kt{0.0f, 0.0f, 0.0f};
    float ktPorcentaje{0.0f};

    Reflectancia(
        Emision kdIn = {0.0f, 0.0f, 0.0f},
        Emision ksIn = {0.0f, 0.0f, 0.0f},
        Emision ktIn = {0.0f, 0.0f, 0.0f}
    ) : kd(kdIn), ks(ksIn), kt(ktIn) {
        float sumaR = kd.r + ks.r + kt.r;
        float sumaG = kd.g + ks.g + kt.g;
        float sumaB = kd.b + ks.b + kt.b;
        if(sumaR > 1.0f){
            kd.r /= sumaR; ks.r /= sumaR; kt.r /= sumaR;
            std::cerr << "Warning: La suma de rojos es mas de 1. Normalizando" << std::endl;
        } 
        if(sumaG > 1.0f){
            kd.g /= sumaG; ks.g /= sumaG; kt.g /= sumaG;
            std::cerr << "Warning: La suma de verdes es mas de 1. Normalizando" << std::endl;
        } 
        if(sumaB > 1.0f){
            kd.b /= sumaB; ks.b /= sumaB; kt.b /= sumaB;
            std::cerr << "Warning: La suma de azules es mas de 1. Normalizando" << std::endl;
        } 

        float maxKd = std::max({kd.r, kd.g, kd.b});
        float maxKs = std::max({ks.r, ks.g, ks.b});
        float maxKt = std::max({kt.r, kt.g, kt.b});
        float totalMax = maxKd + maxKs + maxKt;

        if(totalMax == 0.0f) {
            kdPorcentaje = 0.0f;
            ksPorcentaje = 0.0f;
            ktPorcentaje = 0.0f;
        } else {
            kdPorcentaje = maxKd / totalMax;
            ksPorcentaje = maxKs / totalMax;
            ktPorcentaje = maxKt / totalMax;
        }
    }
};

//Clase Abstracta
class Primitiva {
    public:
        Emision emision;
        Reflectancia reflectancia;
        
        bool esEmisiva;

        float indiceRefraccion;

        explicit Primitiva(Emision emisionIn, Reflectancia reflectanciaIn, float indiceRefraccionIn):emision(emisionIn), reflectancia(reflectanciaIn), indiceRefraccion(indiceRefraccionIn)  {
            if (emision.r > 0 || emision.g > 0 || emision.b > 0) {
                esEmisiva = true;
            } else {
                esEmisiva = false;
            }
        }; 
        virtual bool intersecta(const Ray& ray, PD& puntoInterseccionOut, PD& normalOut) const = 0;

		virtual ~Primitiva() = default;
};

#endif // PRIMITIVA_HPP