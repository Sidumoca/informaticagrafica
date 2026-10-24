#ifndef LUZ_PUNTUAL_HPP
#define LUZ_PUNTUAL_HPP

#include "pd.hpp"
#include <iostream>

using namespace std;

class LuzPuntual {
public:
	PD posicion;
	Emision emision;

	explicit LuzPuntual(PD posicion, Emision emisionIn): posicion(posicion), emision(emisionIn)
	{if(!posicion.isPoint()) cerr << "La posicion de la luz puntual debe ser un punto" << endl; };

	virtual ~LuzPuntual() = default;
}; 

#endif // LUZ_PUNTUAL_HPP