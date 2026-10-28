#ifndef PRIMITIVA_HPP
#define PRIMITIVA_HPP

#include "../geometria/pd.hpp"
#include "../image.hpp"

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
        virtual bool intersecta(const Ray& ray, PD& puntoInterseccionOut, PD& normalOut) const = 0;

		virtual ~Primitiva() = default;
};

#endif // PRIMITIVA_HPP