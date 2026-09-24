#include <Eigen/Core>
#include <Eigen/Geometry>
#include <cmath>
#include <iostream>
#include "pd.hpp"

PD::PD()
{
    vector[0] = 0;
    vector[1] = 0;
    vector[2] = 0;
    vector[3] = 0;
}

PD::PD(float x, float y, float z, float w) {
	vector[0] = x;
	vector[1] = y;
	vector[2] = z;
	vector[3] = w;
}

const float& PD::operator[](int i) const {
	return vector[i];
}

float& PD::operator[](int i) {
	return vector[i];
}

// Suma:
// d + d = d
// p + d = p
// d + p = p
// p + p = inválido
PD PD::operator+(const PD& other) const
{
    // No se pueden sumar dos puntos.
    if (vector[3] == 1 && other.vector[3] == 1) {
		std::cerr << "No se pueden sumar dos puntos" << std::endl;
    }

    return PD(
        vector[0] + other.vector[0],
        vector[1] + other.vector[1],
        vector[2] + other.vector[2],
        vector[3] + other.vector[3]
    );
}


// Resta
// p - p = d
// p - d = p
// d - d = d
// d - p = inválido
PD PD::operator-(const PD& other) const
{
    // Una dirección no se puede restar de un punto:
    if (vector[3] == 0 && other.vector[3] == 1) {
		std::cerr << "No se puede restar un punto de una direccion" << std::endl;
    }

    return PD(
        vector[0] - other.vector[0],
        vector[1] - other.vector[1],
        vector[2] - other.vector[2],
        vector[3] - other.vector[3]
    );
}


// Multiplicación por escalar:
// Solo tiene sentido para direcciones.
// d · s = d
PD PD::operator*(float scalar) const
{
    if (vector[3] != 0) {
		std::cerr << "La multiplicacion por escalar solo esta definida para direcciones" << std::endl;
    }

    return PD(
        vector[0] * scalar,
        vector[1] * scalar,
        vector[2] * scalar,
        0
    );
}


// Para que la multiplicación por un escalar sea conmutativa.
PD operator*(float scalar, const PD& vec)
{
    return vec * scalar;
}


// Division por escalar:
// Solo tiene sentido para direcciones.
// d/s = d
PD PD::operator/(float scalar) const
{
    if (vector[3] != 0) {
		std::cerr << "La division por escalar solo esta definida para direcciones" << std::endl;
    }

    if (scalar == 0) {
		std::cerr << "No se puede dividir por cero" << std::endl;
    }

    return PD(
        vector[0] / scalar,
        vector[1] / scalar,
        vector[2] / scalar,
        0
    );
}


// Producto escalar entre direcciones
float PD::dot(const PD& other) const
{
    if (vector[3] == 0 && other.vector[3] == 0) {
        Eigen::Vector3f v1(vector[0], vector[1], vector[2]);
        Eigen::Vector3f v2(other.vector[0], other.vector[1], other.vector[2]);

        return v1.dot(v2);
    }
	std::cerr << "El producto escalar solo esta definido entre direcciones" << std::endl;
}


// Producto vectorial entre direcciones
PD PD::cross(const PD& other) const
{
    if (vector[3] == 0 && other.vector[3] == 0) {
        Eigen::Vector3f v1(vector[0], vector[1], vector[2]);
        Eigen::Vector3f v2(other.vector[0], other.vector[1], other.vector[2]);

        Eigen::Vector3f result = v1.cross(v2);

        return PD(
            result.x(),
            result.y(),
            result.z(),
            0
        );
    }
	std::cerr << "El producto vectorial solo esta definido entre direcciones" << std::endl;
}


// Módulo de una dirección:
float PD::module() const
{
    if (vector[3] != 0) {
		std::cerr << "El modulo solo esta definido para direcciones" << std::endl;
    }

    Eigen::Vector3f v(vector[0], vector[1], vector[2]);

    return v.norm();
}
