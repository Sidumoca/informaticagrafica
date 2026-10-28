#ifndef TRIANGULO_HPP
#define TRIANGULO_HPP

#include "primitiva.hpp"
#include "../geometria/pd.hpp"
#include "../geometria/ray.hpp"

class Triangulo : public Primitiva {
public:
	PD p1, p2, p3;

	PD dir1, dir2;
	PD normal;
	float distancia;

	Triangulo(PD p1, PD p2, PD p3, Emision emisionIn, Reflectancia reflectanciaIn);
	bool intersecta(const Ray& ray, PD& puntoInterseccionOut, PD& normalOut) const override;
}; 

#endif // TRIANGULO_HPP