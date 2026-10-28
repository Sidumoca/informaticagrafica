#include "camara.hpp"
#include "geometria/ray.hpp"
#include <iostream>
#include <random>
#include <omp.h>

using namespace std;

Camara::Camara(PD origenIn, PD leftIn, PD upIn, PD forwardIn, int widthIn, int heightIn, const Escena& escenaIn, int raysPerPixelIn)
	: escena(escenaIn)
{
	this->origen=origenIn;
	this->left=leftIn;
	this->up=upIn;
	this->forward=forwardIn;
	this->width=widthIn;
	this->height=heightIn;
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
}


Emision Camara::generateRay(int i, int j, float multiplierX, float multiplierY){
	PD direccionRay = (this->puntoPrimerPixel-(uLeft*i)-(uUp*j)) - this->origen;
	direccionRay = direccionRay - uLeft*multiplierX - uUp*multiplierY;
	Ray ray(this->origen, direccionRay);

	PD puntoInterseccion;
	Emision emision;
	ray.intersecta(escena, puntoInterseccion, emision);
	
	return emision;
}

Emision Camara::generateKRays(int i, int j, int k){
	if(k<=0){
		cerr << "El numero de rayos por pixel debe ser mayor o igual a 1" << endl;
		return {0,0,0};
	}

	if(k==1) return this->generateRay(i,j,0.5f,0.5f);
	
	static thread_local std::mt19937 generator(std::random_device{}() + omp_get_thread_num());
	std::uniform_real_distribution<float> distribution(0.0f, 1.0f);

	Emision media={0,0,0};
	for(int h=0;h<k;++h){
		const float multiplierX = distribution(generator);
		const float multiplierY = distribution(generator);
		media = media + this->generateRay(i, j, multiplierX, multiplierY);
	}
	return (media/k);
}

void Camara::imprimirImagen(const char *path){
	Image im(this->width, this->height, true);

	#pragma omp parallel for collapse(2) schedule(dynamic, 64)
	for(int j=0;j<this->height;++j){
		for(int i=0;i<this->width;++i){
			Emision emision = this->generateKRays(i,j, this->raysPerPixel);
			im.image[j][i].r = emision.r;
			im.image[j][i].g = emision.g;
			im.image[j][i].b = emision.b;
		}
	}
	im.curvaGamma(2.2f);
	im.write(path);
}
