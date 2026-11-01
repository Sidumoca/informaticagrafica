// En la carpeta build: "cmake ..", "cmake --build ." y "./main3"

#include <vector>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <memory>
#include <string>
#include "primitivas/triangulo.hpp"
#include "primitivas/esfera.hpp"
#include "primitivas/monolito.hpp"
#include "primitivas/plano.hpp"
#include "primitivas/primitiva.hpp"
#include "geometria/ray.hpp"
#include "geometria/pd.hpp"
#include "camara.hpp"
#include "escena.hpp"
#include "luzPuntual.hpp"

#include "nlohmann/json.hpp"

using nlohmann::json;
namespace fs = std::filesystem;

Emision leerColor(const json& object, const char* nombre) {
    if (!object.contains(nombre)) {
        return {0.0f, 0.0f, 0.0f};
    }

    const json& color = object.at(nombre);
    return {
        color.value("r", 0.0f),
        color.value("g", 0.0f),
        color.value("b", 0.0f)
    };
}

Emision leerEmision(const json& object) {
    return leerColor(object, "emision");
}

Reflectancia leerReflectancia(const json& object) {
    return Reflectancia{
        leerColor(object, "kd"),
        leerColor(object, "ks"),
        leerColor(object, "kt")
    };
}

int main(int argc, char* argv[]) {

	if (argc < 2) {
		std::cerr << "Uso: " << argv[0] << " <archivo.json>\n";
		return 1;
	}

	std::ifstream f(argv[1]);
	json data = json::parse(f);
	
	Escena escena;
	
	
	PD origenCamara(
		data["camara"]["origen"]["x"].get<float>(),
		data["camara"]["origen"]["y"].get<float>(),
		data["camara"]["origen"]["z"].get<float>(),
		1
	);
	PD leftCamara(
		data["camara"]["left"]["x"].get<float>(),
		data["camara"]["left"]["y"].get<float>(),
		data["camara"]["left"]["z"].get<float>(),
		0
	);
	PD upCamara(
		data["camara"]["up"]["x"].get<float>(),
		data["camara"]["up"]["y"].get<float>(),
		data["camara"]["up"]["z"].get<float>(),
		0
	);
	PD forwardCamara(
		data["camara"]["forward"]["x"].get<float>(),
		data["camara"]["forward"]["y"].get<float>(),
		data["camara"]["forward"]["z"].get<float>(),
		0
	);
	int width = data["camara"]["width"].get<int>();
	int height = data["camara"]["height"].get<int>();
	int raysPerPixel = data["camara"]["raysPerPixel"].get<int>();
	
	for(const auto& object : data["primitivas"]){
		std::string type = object["type"];

		if( type == "triangulo"){
			escena.primitivas.push_back(
				std::make_unique<Triangulo>(
					PD(
						object["p1"]["x"].get<float>(),
						object["p1"]["y"].get<float>(),
						object["p1"]["z"].get<float>(),
						1
					),
					PD(
						object["p2"]["x"].get<float>(),
						object["p2"]["y"].get<float>(),
						object["p2"]["z"].get<float>(),
						1
					),
					PD(
						object["p3"]["x"].get<float>(),
						object["p3"]["y"].get<float>(),
						object["p3"]["z"].get<float>(),
						1
					),
					leerEmision(object),
					leerReflectancia(object)
				)
			);
		} else if(type == "esfera"){
			escena.primitivas.push_back(
				std::make_unique<Esfera>(
					PD(
						object["centro"]["x"].get<float>(),
						object["centro"]["y"].get<float>(),
						object["centro"]["z"].get<float>(),
						1
					),
					object["radio"].get<float>(),
					leerEmision(object),
					leerReflectancia(object)
				)
			);
		} else if(type == "monolito"){
			escena.primitivas.push_back(
				std::make_unique<Monolito>(
					PD(
						object["origen"]["x"].get<float>(),
						object["origen"]["y"].get<float>(),
						object["origen"]["z"].get<float>(),
						1
					),
					PD(
						object["i"]["x"].get<float>(),
						object["i"]["y"].get<float>(),
						object["i"]["z"].get<float>(),
						0
					),
					PD(
						object["j"]["x"].get<float>(),
						object["j"]["y"].get<float>(),
						object["j"]["z"].get<float>(),
						0
					),
					PD(
						object["k"]["x"].get<float>(),
						object["k"]["y"].get<float>(),
						object["k"]["z"].get<float>(),
						0
					),
					leerEmision(object),
					leerReflectancia(object)
				)
			);
		} else if(type == "plano"){
			escena.primitivas.push_back(
				std::make_unique<Plano>(
					object["distancia"].get<float>(),
					PD(
						object["normal"]["x"].get<float>(),
						object["normal"]["y"].get<float>(),
						object["normal"]["z"].get<float>(),
						0
					),
					leerEmision(object),
					leerReflectancia(object)
				)
			);
		} else {
        	std::cerr << "Tipo de primitiva desconocido: " << type << std::endl;
    	}
	}

	for (const auto& luz : data["lucesPuntuales"]) {
		escena.lucesPuntuales.push_back(
			std::make_unique<LuzPuntual>(
				PD(
					luz["posicion"]["x"].get<float>(),
					luz["posicion"]["y"].get<float>(),
					luz["posicion"]["z"].get<float>(),
					1
				),
				Emision{
					luz["color"]["r"].get<float>(),
					luz["color"]["g"].get<float>(),
					luz["color"]["b"].get<float>()
				}
			)
		);
	}
	
	Camara camara(origenCamara, leftCamara, upCamara, forwardCamara, width, height, escena, raysPerPixel);
	fs::create_directories("outputs");
	fs::path ficheroEntrada(argv[1]);
	fs::path rutaSalida = fs::path("outputs") / (ficheroEntrada.stem().string() + ".png");
	camara.imprimirImagen(rutaSalida.c_str());
}
