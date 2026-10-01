#ifndef ESFERA_HPP
#define ESFERA_HPP

#include "primitiva.hpp"
#include "pd.hpp"
#include "ray.hpp"

class Esfera : public Primitiva {
public:
	PD centro;
	float radio;

	Esfera(PD centroIn, float radioIn, Emision emisionIn);
	bool intersecta(Ray ray, PD& puntoInterseccionOut) const override;
}; 

#endif // ESFERA_HPP