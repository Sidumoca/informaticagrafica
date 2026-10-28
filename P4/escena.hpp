#ifndef ESCENA_HPP
#define ESCENA_HPP

#include <vector>
#include <memory>

#include "primitivas/primitiva.hpp"
#include "luzPuntual.hpp"

class Escena{
    public:
        std::vector<std::unique_ptr<Primitiva>> primitivas;
        std::vector<std::unique_ptr<LuzPuntual>> lucesPuntuales;
        Escena() = default;
        ~Escena() = default;
};

#endif // ESCENA_HPP