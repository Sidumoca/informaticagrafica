#ifndef LUZ_PUNTUAL_HPP
#define LUZ_PUNTUAL_HPP

#include "pd.hpp"

class LuzPuntual {
public:
	PD posicion;
	Emision emision;

	explicit LuzPuntual(PD posicion, Emision emisionIn): posicion(posicion), emision(emisionIn)
	{if(!posicion.isPoint()) cerr << "La posicion de la luz puntual debe ser un punto" << endl; };ç

	virtual ~LuzPuntual() = default;
}; 

#endif // LUZ_PUNTUAL_HPP