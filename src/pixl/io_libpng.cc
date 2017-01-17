//
// Copyright (c) 2017. See AUTHORS file.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
#include <iostream>

#include <png.h>

#include "io.h"
#include "image.h"

#define LOG(msg) std::cout << msg << std::endl

namespace pixl {
	
    // ----------------------------------------------------------------------------
	FILE* openAndVerifyHeader(const char* path) {
		// open file
        FILE *file = fopen(path, "rb");
        if(!file) {
        	LOG("Failed to read " << path);
        	return 0;
        }
        
        // verify header
        fread(header, 1, 8, file);
        png_byte header[8];  
        if(png_sig_cmp(header, 0, 8)) {
        	LOG(path << " is not a valid png image");
        	return 0;
        }

        return file;
	}

    // ----------------------------------------------------------------------------
    Image* PngReader::read(const char* path) {
        FILE* file = openAndVerifyHeader(path);

        // setup
        png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
        if (!png_ptr) {
            LOG("png_create_read_struct failed");
            return 0;
        }

        png_infop info_ptr = png_create_info_struct(png_ptr);
        if (!info_ptr) {
        	LOG("png_create_info_struct failed");
        }

        if (setjmp(png_jmpbuf(png_ptr))) {
        	LOG("Error during init_io");
        	return 0;
        }

        png_init_io(png_ptr, file);
        png_set_sig_bytes(png_ptr, 8);
        png_read_info(png_ptr, info_ptr);

        i32 width = png_get_image_width(png_ptr, info_ptr);
        i32 height = png_get_image_height(png_ptr, info_ptr);
        png_byte channels = png_get_channels(png_ptr, info_ptr);
        //png_byte color_type = png_get_color_type(png_ptr, info_ptr);
        //png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);
        png_read_update_info(png_ptr, info_ptr);

        // read file
        if (setjmp(png_jmpbuf(png_ptr))) {
        	LOG("Error during read_image");
        }

		png_bytep  	row_pointers[height];
        u32 		rowbytes = png_get_rowbytes(png_ptr, info_ptr);
        u8* 		image_data = (u8*) malloc(rowbytes * height);

        for (int i = 0;  i < height;  ++i) {
        	row_pointers[i] = image_data + i * rowbytes;
        }

        png_read_image(png_ptr, row_pointers);
        png_read_end(png_ptr, NULL);
        fclose(file);

        return new Image(width, height, channels, image_data);
    }

    // ----------------------------------------------------------------------------
    void PngWriter::write(const char* path, Image* image) {
    	// TODO implement
    }
}