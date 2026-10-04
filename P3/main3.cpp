#include <vector>

#include "esfera.hpp"
#include "plano.hpp"
#include "primitiva.hpp"
#include "ray.hpp"
#include "pd.hpp"

using namespace std;

vector<Primitiva*> primitivas;


int main() {
	Esfera* esfera = new Esfera();
	Plano* plano = new Plano();

	primitivas.push_back(esfera);
	primitivas.push_back(plano);

	Ray rayo();

	PD puntoInterseccion;
	if (rayo.intersecta(primitivas, puntoInterseccion)) {
		cout << "Interseccion encontrada en: " << puntoInterseccion.x << ", " << puntoInterseccion.y << ", " << puntoInterseccion.z << endl;
	} else {
		cout << "No hay interseccion con las primitivas." << endl;
	}

	//destruir primitivasss que bien que me he acordado
	for (auto& primitiva : primitivas) {
		delete primitiva;
	}
	primitivas.clear();
}
