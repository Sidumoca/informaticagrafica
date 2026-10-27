#ifndef PLANO_HPP
#define PLANO_HPP

#include "primitiva.hpp"
#include "pd.hpp"
#include "ray.hpp"

class Plano : public Primitiva {
public:
    float distancia;
    PD normal;

	Plano(float distanciaIn, PD normalIn, Emision emisionIn, Reflectancia reflectanciaIn);
	bool intersecta(const Ray& ray, PD& puntoInterseccionOut, PD& normalOut) const override;
}; 

#endif // PLANO_HPP