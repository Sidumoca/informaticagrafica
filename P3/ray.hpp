#ifndef RAY_HPP
#define RAY_HPP

#include "pd.hpp"

class Ray {
public:
    PD origen;
    PD direccion;

    Ray(PD origenIn, PD direccionIn);
};

#endif // RAY_HPP
