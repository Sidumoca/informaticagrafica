#ifndef MONOLITO_HPP
#define MONOLITO_HPP

#include "primitiva.hpp"
#include "pd.hpp"
#include "ray.hpp"

class Monolito : public Primitiva {
public:
	PD centro;
	PD i,j,k;
	PD uI, uJ, uK;
	float mediaLen[3];


	Monolito(PD centro, PD i, PD j, PD k, Emision emisionIn, Reflectancia reflectanciaIn);
	bool intersecta(Ray ray, PD& puntoInterseccionOut) const override;
}; 

#endif // MONOLITO_HPP