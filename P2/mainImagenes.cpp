#include "image.hpp"

int main(){
	Image im("../../PrismsLenses.exr", true);
	//im.clamping();
	im.write("test.png");
}