#ifndef PRIMITIVA_HPP
#define PRIMITIVA_HPP

#include "pd.hpp"
#include "image.hpp"

class Ray; // Declaración adelantada porque sino hay dependencia circular.

//Clase Abstracta
class Primitiva {
    public:
        Emision emision;

        explicit Primitiva(Emision emisionIn):emision(emisionIn) {}; 
        virtual bool intersecta(Ray ray, PD& puntoInterseccionOut) const = 0;

		virtual ~Primitiva() = default;
};

#endif // PRIMITIVA_HPP