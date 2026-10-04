#ifndef RAY_HPP
#define RAY_HPP

#include "pd.hpp"
#include "primitiva.hpp"
#include <vector>

class Ray {
public:
    PD origen;
    PD direccion;

    Ray(PD origenIn, PD direccionIn);

    bool intersecta(const std::vector<Primitiva*>& primitivas, PD& puntoInterseccionOut) const;
};

#endif // RAY_HPP
