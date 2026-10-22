#ifndef PRIMITIVA_HPP
#define PRIMITIVA_HPP

#include "pd.hpp"
#include "image.hpp"
#include <math.h>

class Ray; // Declaración adelantada porque sino hay dependencia circular.

struct Reflectancia {
    float kd;
};

//Clase Abstracta
class Primitiva {
    public:
        Emision emision;
        Reflectancia reflectancia;

        explicit Primitiva(Emision emisionIn, Reflectancia reflectanciaIn):emision(emisionIn), reflectancia(reflectanciaIn)  {}; 
        virtual bool intersecta(Ray ray, PD& puntoInterseccionOut) const = 0;
        Emision devolverEmision(){
            return emision*reflectancia.kd/M_PI;
        }

		virtual ~Primitiva() = default;
};

#endif // PRIMITIVA_HPP