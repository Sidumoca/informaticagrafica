#include "image.hpp"


Image::Image(const char *path, bool isHDRIN)
    :isHDR(isHDRIN)
{
	this->read(path);
}

void Image::write(const char *path){
	if(this->isHDR){

        Array2D<Rgba> pixels(height, width);
        for (unsigned int y=0; y<height; y++)
        {
            for (unsigned int x=0; x<width; x++)
                pixels[y][x] = Rgba(
                    image[y][x].Rojo,
                    image[y][x].Verde,
                    image[y][x].Azul
                );
        }

        try {
            RgbaOutputFile file (path, width, height, WRITE_RGBA);
            file.setFrameBuffer (&pixels[0][0], 1, width);
            file.writePixels (height);
        } catch (const std::exception &e) {
            std::cerr << "Error escribiendo " << path << ": " << e.what() << std::endl;
        }

	} else { //es LDR
		FILE *fp = fopen(path, "wb");
		png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
		png_infop info_ptr = png_create_info_struct(png_ptr);
		if (!png_ptr || !info_ptr) {
			fclose(fp);
			return;
		}
		png_init_io(png_ptr, fp);
		png_set_IHDR(
			png_ptr,
			info_ptr,
			width,
			height,
			8,
			PNG_COLOR_TYPE_RGBA,
			PNG_INTERLACE_NONE,
			PNG_COMPRESSION_TYPE_DEFAULT,
			PNG_FILTER_TYPE_DEFAULT
		);
		std::vector<std::vector<png_byte>> pixels(
			height, std::vector<png_byte>(width * 4)
		);
		std::vector<png_bytep> rows(height);

		for (std::size_t y = 0; y < height; ++y) {
			rows[y] = pixels[y].data();
			for (std::size_t x = 0; x < width; ++x) {
				rows[y][4 * x + 0] = static_cast<png_byte>(image[y][x].Rojo);
				rows[y][4 * x + 1] = static_cast<png_byte>(image[y][x].Verde);
				rows[y][4 * x + 2] = static_cast<png_byte>(image[y][x].Azul);
				rows[y][4 * x + 3] = 255;
			}
		}

		png_set_rows(png_ptr, info_ptr, rows.data());
		png_write_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);
		png_destroy_write_struct(&png_ptr, &info_ptr);
		fclose(fp);
	}
}
void Image::read(const char *path){
	if(this->isHDR){

//----------------------------------------------------
        try
        {
            Imf::RgbaInputFile file(path);

            Imath::Box2i dw = file.dataWindow();

            width  = dw.max.x - dw.min.x + 1;
            height = dw.max.y - dw.min.y + 1;

            Imf::Array2D<Imf::Rgba> pixels(height, width);

            file.setFrameBuffer(
                &pixels[0][0],
                1,
                width
            );

            file.readPixels(dw.min.y, dw.max.y);

            image.resize(height);

            for (unsigned int y = 0; y < height; y++)
            {
                image[y].resize(width);

                for (unsigned int x = 0; x < width; x++)
                {
                    image[y][x].Rojo  = pixels[y][x].r;
                    image[y][x].Verde = pixels[y][x].g;
                    image[y][x].Azul  = pixels[y][x].b;
                }
            }
        }
        catch (const std::exception& e)
        {
            std::cerr << "Error leyendo la imagen "
                      << path << ": "
                      << e.what()
                      << std::endl;
        }
//----------------------------------------------------
    
	} else { // es LDR
		FILE *fp = fopen(path, "rb");
		if (!fp) {
			return;
		}
		png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
		png_infop info_ptr = png_create_info_struct(png_ptr);
		if (!png_ptr || !info_ptr) {
			fclose(fp);
			return;
		}

		png_init_io(png_ptr, fp);
		png_read_info(png_ptr, info_ptr);
		//NORMALIZAR IMAGEN-----------------------------------

		// Normalize every input to 8-bit RGB.
		png_uint_32 input_width, input_height;
		int bit_depth, color_type;
		png_get_IHDR(png_ptr, info_ptr, &input_width, &input_height, &bit_depth, &color_type, nullptr, nullptr, nullptr);

		if (color_type == PNG_COLOR_TYPE_PALETTE) {
			png_set_palette_to_rgb(png_ptr); // Convert indexed color to RGB
		}
		if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8) {
			png_set_expand_gray_1_2_4_to_8(png_ptr); // Expand low-bit grayscale to 8-bit
		}
		if (bit_depth == 16) {
			png_set_strip_16(png_ptr); // Scale 16-bit down to 8-bit
		}
		if (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA) {
			png_set_gray_to_rgb(png_ptr); // Convert grayscale to RGB
		}
		if (color_type & PNG_COLOR_MASK_ALPHA) {
			png_set_strip_alpha(png_ptr); //eliminar alpha
		}
		//--------------------------------------------------------------------

		png_read_update_info(png_ptr, info_ptr);
		width = png_get_image_width(png_ptr, info_ptr);
		height = png_get_image_height(png_ptr, info_ptr);

		std::vector<std::vector<png_byte>> pixels(
			height, std::vector<png_byte>(width * 3)
		);
		std::vector<png_bytep> rows(height);
		for (std::size_t y = 0; y < height; ++y) {
			rows[y] = pixels[y].data();
		}
		png_read_image(png_ptr, rows.data());
		
		image.resize(height);

		for (std::size_t y = 0; y < height; ++y) {
			image[y].resize(width);

			for (std::size_t x = 0; x < width; ++x) {
				image[y][x].Rojo = pixels[y][3 * x + 0];
				image[y][x].Verde = pixels[y][3 * x + 1];
				image[y][x].Azul = pixels[y][3 * x + 2];
			}
		}
		
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		fclose(fp);
	}
}


void Image::clamping(){
	if(!isHDR){
        std::cerr << "No puedes transformar una imagen LDR" << std::endl;		
		return;
	}
	isHDR = false;
	for(unsigned i=0;i<height;++i){
		for(unsigned j=0;j<width;++j){
			image[i][j].Rojo = (image[i][j].Rojo > 255) ? 255 : image[i][j].Rojo;
			image[i][j].Verde = (image[i][j].Verde > 255) ? 255 : image[i][j].Verde;
			image[i][j].Azul = (image[i][j].Azul > 255) ? 255 : image[i][j].Azul;
		}
	}
}
