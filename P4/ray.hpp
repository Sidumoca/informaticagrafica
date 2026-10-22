#ifndef RAY_HPP
#define RAY_HPP

#include "pd.hpp"
#include "primitiva.hpp"
#include "image.hpp"
#include "escena.hpp"
#include <vector>

class Ray {
public:
    PD origen;
    PD direccion;

    Ray(PD origenIn, PD direccionIn);

    bool intersecta(const Escena escena, PD& puntoInterseccionOut, Emision& emisionOut) const;
};

#endif // RAY_HPP
