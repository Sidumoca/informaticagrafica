#include "ray.hpp"
#include <limits>

using namespace std;

Ray::Ray(PD origenIn, PD direccionIn) 
        : origen(origenIn), direccion(direccionIn) 
{
    if (direccionIn.isPoint()) {
        cerr << "La direccion del rayo debe ser un vector" << endl;
    }
    else if(origenIn.isPoint()){
        cerr << "El origen del rayo debe ser un punto" << endl;
    }
}

    bool intersecta(const vector<Primitiva*>& primitivas, PD& puntoInterseccionOut) const{
		bool interseccionEncontrada = false;
		float distanciaMinima = numeric_limits<float>::max(); //maximo posible valor en floats
		PD puntoInterseccionTemp;

		for (const auto& primitiva : primitivas) {
			if (primitiva->intersecta(*this, puntoInterseccionTemp)) {
				float distancia = (puntoInterseccionTemp - origen).module();
				if (distancia < distanciaMinima) {
					distanciaMinima = distancia;
					puntoInterseccionOut = puntoInterseccionTemp;
					interseccionEncontrada = true;
				}
			}
		}

		return interseccionEncontrada;
	}

