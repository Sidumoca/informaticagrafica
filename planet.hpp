#ifndef PLANET_HPP
#define PLANET_HPP

#include "pd.hpp"

class Planet {
	public:
		Planet(PD eje, PD ciudad, PD centro, float inclinacion, float azimut);
		PD eje; //dirección
		PD ciudad; // punto
		PD centro; //punto

		PD dirCiudad;

		float inclinacion; //inclinacion estacion. 0 -> pi
		float azimut; //azimut estacion. -pi -> pi

		PD estacion; //punto
		
		//direcciones
		PD normalEstacion;
		PD tangenteLongitud;
		PD tangenteLatitud;

};

#endif // PLANET_HPP