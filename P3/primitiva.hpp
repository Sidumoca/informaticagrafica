#pragma once
#include 

#include "pd.hpp"
#include "ray.hpp"

struct Emision {
	float r;
	float g;
	float b;
};

//Clase Abstracta
class Primitiva {
    public:
        Emision emision;

        explicit Primitiva(Emision emisionIn):emision(emisionIn) {}; 
        virtual bool intersecta(Ray ray, PD& puntoInterseccionOut) const = 0;

		virtual ~Primitiva() = default;
};