#ifndef TRIANGULO_HPP
#define TRIANGULO_HPP

#include "primitiva.hpp"
#include "pd.hpp"
#include "ray.hpp"

class Triangulo : public Primitiva {
public:
	PD p1, p2, p3;

	Triangulo(PD p1, PD p2, PD p3, Emision emisionIn, Reflectancia reflectanciaIn);
	bool intersecta(Ray ray, PD& puntoInterseccionOut, PD& normalOut) const override;
	PD getNormal(const PD& punto) const override;
}; 

#endif // TRIANGULO_HPP