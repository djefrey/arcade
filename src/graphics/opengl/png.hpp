/*
** EPITECH PROJECT, 2022
** B-OOP-400-STG-4-1-arcade-nykyta.kudrya
** File description:
** png
*/

#pragma once

#include <string>
#include <stdexcept>
#include <png.h>
#include <stdio.h>
#include <string.h>

#include <iostream>

namespace ogl {
    class PNG {
        public:
        static uint8_t *readPNGFile(const std::string &filename, uint *width, uint *height)
        {
            FILE *file = fopen(filename.c_str(), "rb");
            png_struct *png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
            png_info *info_ptr = png_create_info_struct(png_ptr);
            uint8_t *data;
            uint rowbytes;

            if (file == nullptr)
                throw std::runtime_error("Could not open " + filename);

            png_init_io(png_ptr, file);
            png_set_sig_bytes(png_ptr, 0);
            png_read_info(png_ptr, info_ptr);

            int test;
            std::cout << filename << ": " << png_get_sRGB(png_ptr, info_ptr, &test) << std::endl;
            std::cout << test << std::endl;

            *width = png_get_image_width(png_ptr, info_ptr);
            *height = png_get_image_height(png_ptr, info_ptr);
            rowbytes = 4 * sizeof(uint8_t) * *width;

            char color_type = png_get_color_type(png_ptr, info_ptr);
            if (!(color_type & PNG_COLOR_MASK_ALPHA))
                png_set_add_alpha(png_ptr, 255, PNG_FILLER_AFTER);
            else if (!(color_type & PNG_COLOR_MASK_COLOR))
                throw std::runtime_error("Grayscale PNG not supported");

            data = new uint8_t[rowbytes * *height];
            for (int i = 0; i < *height; i++)
                png_read_row(png_ptr, data + i * rowbytes, NULL);

            png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

            return data;
        }
    };
}