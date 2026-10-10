#include "camara.hpp"
#include "ray.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

Camara::Camara(PD origenIn, PD leftIn, PD upIn, PD forwardIn, int widthIn, int heightIn, std::vector<Primitiva*> primitivasIn, int raysPerPixelIn){
	this->origen=origenIn;
	this->left=leftIn;
	this->up=upIn;
	this->forward=forwardIn;
	this->width=widthIn;
	this->height=heightIn;
	this->primitivas=primitivasIn;
	this->raysPerPixel=raysPerPixelIn;

	//precalculamos cositas
	this->anchoDePixel=2*this->left.module()/width;
	this->altoDePixel =2*this->up.module()/height;
	if(anchoDePixel!=altoDePixel)
		std::cout << "El ancho y largo de pixel debe ser igual. Vigila la relacion entre up y left." << std::endl;

	this->uLeft = this->left/(this->width/2);
	this->uUp = this->up/(this->height/2);

	this->puntoPrimerPixel=this->origen+this->forward+this->left+this->up;
	//this->puntoPrimerPixel = puntoPrimerPixel - this->uLeft/2 - this->uUp/2; //para ponerlo en el centro

	srand(static_cast<unsigned int>(time(nullptr)));

}


Emision Camara::generateRay(int i, int j, float multiplierX, float multiplierY){
	PD direccionRay = (this->puntoPrimerPixel-(uLeft*i)-(uUp*j)) - this->origen;
	direccionRay = direccionRay - uLeft*multiplierX - uUp*multiplierY;
	Ray ray(this->origen, direccionRay);

	PD puntoInterseccion;
	Emision emision;
	ray.intersecta(primitivas, puntoInterseccion, emision);
	//if(!choca && (i==127 || i==128)) cout << "El rayo (" << i << "," << j << ") le ha fallado a todo! Se ha lanzado a " << ray.direccion << endl;
	return emision;
}

Emision Camara::generateKRays(int i, int j, int k){
	if(k<=0){
		cerr << "El numero de rayos por pixel debe ser mayor o igual a 1" << endl;
		Emision em={0,0,0};
		return em;
	}
	Emision media={0,0,0};
	for(int h=0;h<k;++h){
		const float multiplierX =
			static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
		const float multiplierY =
			static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
		media = media + this->generateRay(i, j, multiplierX, multiplierY);
	}
	return (media/k);
}

void Camara::imprimirImagen(const char *path){
	Image im(this->width, this->height, true);
	for(int i=0;i<this->width;++i){
		for(int j=0;j<this->height;++j){
			Emision emision = this->generateKRays(i,j, this->raysPerPixel); //TODO: añadir parametro k
			im.image[j][i].r = emision.r;
			im.image[j][i].g = emision.g;
			im.image[j][i].b = emision.b;
		}
	}
	im.curvaGamma(2.2f);
	im.write(path);
}
