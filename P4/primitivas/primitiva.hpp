#ifndef PRIMITIVA_HPP
#define PRIMITIVA_HPP

#include "../geometria/pd.hpp"
#include "../image.hpp"

class Ray; // Declaración adelantada porque sino hay dependencia circular.

struct Reflectancia {
    Emision kd{0.0f, 0.0f, 0.0f};
    float kdPorcentaje{0.0f};
    Emision ks{0.0f, 0.0f, 0.0f};
    float ksPorcentaje{0.0f};
    Emision kt{0.0f, 0.0f, 0.0f};
    float ktPorcentaje{0.0f};

    Reflectancia(
        Emision kdIn = {0.0f, 0.0f, 0.0f},
        Emision ksIn = {0.0f, 0.0f, 0.0f},
        Emision ktIn = {0.0f, 0.0f, 0.0f}
    ) : kd(kdIn), ks(ksIn), kt(ktIn) {
        const float kdPeso = luminancia(kd);
        const float ksPeso = luminancia(ks);
        const float ktPeso = luminancia(kt);
        const float pesoTotal = kdPeso + ksPeso + ktPeso;

        if (pesoTotal > 0.0f) {
            kdPorcentaje = kdPeso / pesoTotal;
            ksPorcentaje = ksPeso / pesoTotal;
            ktPorcentaje = ktPeso / pesoTotal;
        }
    }

private:
    static float luminancia(const Emision& color) {
        // Estos valores son los pesos estandarizados RGB para aproximar la
        // luminancia percibida por el ojo humano: el verde aporta más y el
        // azul menos a la percepción del brillo.
        return 0.2126f * color.r + 0.7152f * color.g + 0.0722f * color.b;
    }
};

//Clase Abstracta
class Primitiva {
    public:
        Emision emision;
        Reflectancia reflectancia;
        
        bool esEmisiva;

        explicit Primitiva(Emision emisionIn, Reflectancia reflectanciaIn):emision(emisionIn), reflectancia(reflectanciaIn)  {
            if (emision.r > 0 || emision.g > 0 || emision.b > 0) {
                esEmisiva = true;
            } else {
                esEmisiva = false;
            }
        }; 
        virtual bool intersecta(const Ray& ray, PD& puntoInterseccionOut, PD& normalOut) const = 0;

		virtual ~Primitiva() = default;
};

#endif // PRIMITIVA_HPP