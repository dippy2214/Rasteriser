#pragma once
#include <stdint.h>
#include <vector>
#include <iostream>
#include <fstream>
#include "FloatTypes.h"



#pragma pack(push, 1)
struct BMPFileHeader
{
    uint16_t file_type{ 0x4D42 }; //always binary for BM
    uint32_t file_size{ 0 }; //size of file (in bytes)
    uint16_t reserved1{ 0 }; //always 0
    uint16_t reserved2{ 0 }; //always 0
    uint32_t offset_data{ 0 }; //start position of pixel data (bytes from beginning of file)
};

struct BMPInfoHeader {
    uint32_t size{ 0 };                      // Size of this header (in bytes)
    int32_t width{ 0 };                      // width of bitmap in pixels
    int32_t height{ 0 };                     // height of bitmap in pixels
    // (if positive, bottom-up, with origin in lower left corner)
    // (if negative, top-down, with origin in upper left corner)
    uint16_t planes{ 1 };                    // No. of planes for the target device, this is always 1
    uint16_t bit_count{ 0 };                 // No. of bits per pixel
    uint32_t compression{ 0 };               // 0 or 3 - uncompressed. THIS PROGRAM CONSIDERS ONLY UNCOMPRESSED BMP images
    uint32_t size_image{ 0 };                // 0 - for uncompressed images
    int32_t x_pixels_per_meter{ 0 };
    int32_t y_pixels_per_meter{ 0 };
    uint32_t colors_used{ 0 };               // No. color indexes in the color table. Use 0 for the max number of colors allowed by bit_count
    uint32_t colors_important{ 0 };          // No. of colors used for displaying the bitmap. If 0 all colors are required
};

struct BMPColorHeader
{
    uint32_t red_mask{ 0x00ff0000 };
    uint32_t green_mask{ 0x0000ff00 };
    uint32_t blue_mask{ 0x000000ff };
    uint32_t alpha_mask{ 0xff000000 };
    uint32_t color_space_type{ 0x73524742 };
    uint32_t unused[16]{ 0 };
};
#pragma pack(pop)

class BMPImage
{
public:
    BMPFileHeader file_header;
    BMPInfoHeader info_header;
    BMPColorHeader color_header;
    std::vector<uint8_t> data;

    BMPImage(const char* fname);
    BMPImage(int32_t width, int32_t height, bool has_alpha = true);
    BMPImage(float2 size, bool has_alpha = true);

    void read(const char* fname);
    void write(const char* fname);

    void set_pixel(float x, float y, float4 color);
    float4 get_pixel(float x, float y);
    void clear_image();
private:

    uint32_t row_stride{ 0 };

    uint32_t make_stride_aligned(uint32_t align_stride);

    void check_color_header(BMPColorHeader& bmp_color_header);
    void write_headers(std::ofstream& of);
    void write_headers_and_data(std::ofstream& of);
};

