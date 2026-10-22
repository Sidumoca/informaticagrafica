#ifndef ESCENA_HPP
#define ESCENA_HPP

#include <vector>

#include "primitiva.hpp"
#include "luzPuntual.hpp"

class Escena{
    public:
        std::vector<Primitiva*> primitivas;
        std::vector<LuzPuntual*> lucesPuntuales;
        Escena();
        ~Escena(){
			primitivas.clear();
			lucesPuntuales.clear();
		} //Hacer que borre todos los punteros de los vectores
}

#endif // ESCENA_HPP