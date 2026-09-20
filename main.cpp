#include <iostream>
#include <cmath>
#include "planet.hpp"

void imprimirPD(const PD& p)
{
    std::cout << "(" << p[0] << ", " << p[1] << ", " << p[2] << ", " << p[3] << ")";
}

// ---------------------------------------------------------
// Calcular la posicion de una estacion en el sistema local
// de otra estacion.
// El origen del sistema local es la propia estacion.
// ---------------------------------------------------------
void calcularCoordenadasLocales(const PD& estacionObservada, const Planet& estacionOrigen, float& x, float& y, float& z)
{
    PD vector = estacionObservada - estacionOrigen.estacion;
    // Proyeccion sobre la base local
    x = vector.dot(estacionOrigen.tangenteLongitud);
    y = vector.dot(estacionOrigen.tangenteLatitud);
    z = vector.dot(estacionOrigen.normalEstacion);
}

void mostrarEstacionesLocal(const Planet& A, const Planet& B)
{
    float xA, yA, zA;
    float xB, yB, zB;

    // B en el sistema local de A
    calcularCoordenadasLocales(B.estacion, A, xA, yA, zA);

    // A en el sistema local de B
    calcularCoordenadasLocales(A.estacion, B, xB, yB, zB);

    std::cout << "\n========================================\n";
    std::cout << "       TRAYECTORIA EN SISTEMAS LOCALES\n";
    std::cout << "========================================\n";

    // Sistema local de A
    std::cout << "\nSistema local de la estacion A:\n";
    std::cout << "  A = (0, 0, 0)\n";
    std::cout << "  B = (" << xA << ", " << yA << ", " << zA << ")\n";

    // Sistema local de B
    std::cout << "\nSistema local de la estacion B:\n";
    std::cout << "  B = (0, 0, 0)\n";
    std::cout << "  A = (" << xB << ", " << yB << ", " << zB << ")\n";
}

bool calcularTrayectoriaUnidireccional(Planet& A, Planet& B){
	PD dirLanzamiento = B.estacion - A.estacion;
	dirLanzamiento = dirLanzamiento / dirLanzamiento.module();

	//descomponemos la direccion de lanzamiento en las tres componentes
	float x = dirLanzamiento.dot(A.tangenteLongitud);
	float y = dirLanzamiento.dot(A.tangenteLatitud);
	float z = dirLanzamiento.dot(A.normalEstacion);

	// Si la z es negativa, significa que está yendo hacia el interior del planeta, y por tanto, hay colisión
	if(z<0){
		return false;
	}

    return true;
}

bool calcularTrayectoria(Planet& A, Planet& B){
    bool ok1 = calcularTrayectoriaUnidireccional(A,B);
	if(!ok1){
		std::cerr << "Colision con el planeta de origen" << std::endl;
	}
	bool ok2 = calcularTrayectoriaUnidireccional(B,A);
	if(!ok2){
		std::cerr << "Colision con el planeta de destino" << std::endl;
	}
	return ok1 && ok2;
}

int main(){

	// -----------------------------------------------------
    // PLANETA A
    // -----------------------------------------------------

    PD ejeA(0, 0, 20, 0);
    PD ciudadA(10, 0, 0, 1);
    PD centroA(0, 0, 0, 1);
    // Posicion de la estacion
    float inclinacionA = M_PI / 3;   // 60 grados
    float azimutA = 0;


    // -----------------------------------------------------
    // PLANETA B
    // -----------------------------------------------------

    PD ejeB(0, 0, 20, 0);
    PD ciudadB(60, 0, 0, 1);
    PD centroB(50, 0, 0, 1);
    // Posicion de la estacion
    float inclinacionB = M_PI / 2;   // 90 grados
    float azimutB = M_PI;            // 180 grados
	//COlision con:
		// float inclinacionB = 2 * PI / 3;   // 120 grados
		// float azimutB = PI / 2;            // 90 grados

    // =====================================================
    // CREAR LOS PLANETAS
    // =====================================================

    Planet A(ejeA, ciudadA, centroA, inclinacionA, azimutA);
    Planet B(ejeB, ciudadB, centroB, inclinacionB, azimutB);

	std::cout << "========================================\n";
    std::cout << "       DATOS DE LAS ESTACIONES\n";
    std::cout << "========================================\n";

	// -----------------------------------------------------
    // Estacion A
    // -----------------------------------------------------

	std::cout << "\nESTACION A\n";

    std::cout << "Posicion en UCS: ";
    imprimirPD(A.estacion);
    std::cout << "\n";

    std::cout << "Normal: ";
    imprimirPD(A.normalEstacion);
    std::cout << "\n";

    std::cout << "Tangente longitud: ";
    imprimirPD(A.tangenteLongitud);
    std::cout << "\n";

    std::cout << "Tangente latitud: ";
    imprimirPD(A.tangenteLatitud);
    std::cout << "\n";

	// -----------------------------------------------------
    // Estacion B
    // -----------------------------------------------------

    std::cout << "\nESTACION B\n";

    std::cout << "Posicion en UCS: ";
    imprimirPD(B.estacion);
    std::cout << "\n";

    std::cout << "Normal: ";
    imprimirPD(B.normalEstacion);
    std::cout << "\n";

    std::cout << "Tangente longitud: ";
    imprimirPD(B.tangenteLongitud);
    std::cout << "\n";

    std::cout << "Tangente latitud: ";
    imprimirPD(B.tangenteLatitud);
    std::cout << "\n";

	// -----------------------------------------------------
    // Trayectoria
    // -----------------------------------------------------

	mostrarEstacionesLocal(A,B);
	bool ok = calcularTrayectoria(A,B);

    std::cout << "\n========================================\n";
    std::cout << "             RESULTADO FINAL\n";
    std::cout << "========================================\n";

    if (ok)
    {
        std::cout << "La trayectoria es valida en ambas estaciones.\n";
    }
    else
    {
        std::cout << "La trayectoria presenta una posible colision.\n";
    }

}