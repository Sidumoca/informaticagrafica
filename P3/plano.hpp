#ifndef PLANO_HPP
#define PLANO_HPP

#include "primitiva.hpp"
#include "pd.hpp"
#include "ray.hpp"

class Plano : public Primitiva {
public:
    float distancia;
    PD normal;

	Plano(float distanciaIn, PD normalIn, Emision emisionIn);
	bool intersecta(Ray ray, PD& puntoInterseccionOut) const override;
}; 

#endif // PLANO_HPP