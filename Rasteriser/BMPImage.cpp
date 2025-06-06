#include "BMPImage.h"
#include <iostream>
#include <fstream>

BMPImage::BMPImage(const char* fname)
{
    read(fname);
}



BMPImage::BMPImage(int32_t width, int32_t height, bool alpha)
{
    if (width <= 0 || height <= 0)
    {
        throw std::runtime_error("The image width and height must be positive numbers");
    }

    info_header.width = width;
    info_header.height = height;
    if (alpha)
    {
        info_header.size = sizeof(BMPInfoHeader) + sizeof(BMPColorHeader);
        file_header.offset_data = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader) + sizeof(BMPColorHeader);

        info_header.bit_count = 32;
        info_header.compression = 3;
        row_stride = width * 4;
        data.resize(row_stride * height);
        file_header.file_size = file_header.offset_data + data.size();
    }
    else
    {
        info_header.size = sizeof(BMPInfoHeader);
        file_header.offset_data = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader);

        info_header.bit_count = 24;
        info_header.compression = 0;
        row_stride = width * 3;
        data.resize(row_stride * height);

        uint32_t new_stride = make_stride_aligned(4);
        file_header.file_size = file_header.offset_data + data.size() + info_header.height * (new_stride - row_stride);
    }
}

void BMPImage::read(const char* fname)
{
    std::ifstream inp{ fname, std::ios_base::binary };
    if (inp)
    {
        inp.read((char*)&file_header, sizeof(file_header));
        if (file_header.file_type != 0x4D42)
        {
            throw std::runtime_error("Error! Unrecognised file format.");
        }
        inp.read((char*)&info_header, sizeof(info_header));

        //color header only used for transparent images
        if (info_header.bit_count == 32)
        {
            //check if file has bit mask color info
            if (info_header.size >= (sizeof(BMPInfoHeader) + sizeof(BMPColorHeader)))
            {
                inp.read((char*)&color_header, sizeof(color_header));

                //check if pixel data is stored as BGRA
                check_color_header(color_header);
            }
            else
            {
                std::cerr << "Warning! the file \"" << fname << "\" does not seem to contain bitmask information\n";
                throw std::runtime_error("Error! Unrecognised file format.");
            }
        }


        //jump to pixel data location
        inp.seekg(file_header.offset_data, inp.beg);

        //adjust the header fields for output
        //some editors put extra info in the image file
        if (info_header.bit_count == 32)
        {
            info_header.size = sizeof(BMPInfoHeader) + sizeof(BMPColorHeader);
            file_header.offset_data = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader) + sizeof(BMPColorHeader);
        }
        else
        {
            info_header.size = sizeof(info_header);
            file_header.offset_data = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader);
        }
        file_header.file_size = file_header.offset_data;

        if (info_header.height < 0)
        {
            throw std::runtime_error("The program only supports BMP images with the origin in the bottom left");
        }
        data.resize(info_header.width * info_header.height * info_header.bit_count / 8);

        //check if we need to take into account row padding
        if (info_header.width % 4 == 0)
        {
            inp.read((char*)data.data(), data.size());
            file_header.file_size += data.size();
        }
        else
        {
            row_stride = info_header.width * info_header.bit_count / 8;
            uint32_t new_stride = make_stride_aligned(4);
            std::vector<uint8_t> padding_row(new_stride - row_stride);

            for (int y = 0; y < info_header.height; ++y)
            {
                inp.read((char*)(data.data() + row_stride * y), row_stride);
                inp.read((char*)padding_row.data(), padding_row.size());
            }
            file_header.file_size += data.size() + info_header.height + padding_row.size();
        }
    }
    else
    {
        throw std::runtime_error("Unable to open input image file.");
    }
}

void BMPImage::write(const char* fname)
{
    std::ofstream of{ fname, std::ios_base::binary };
    if (of) {
        if (info_header.bit_count == 32)
        {
            write_headers_and_data(of);
        }
        else if (info_header.bit_count == 24)
        {
            if (info_header.width % 4 == 0)
            {
                write_headers_and_data(of);
            }
            else
            {
                uint32_t new_stride = make_stride_aligned(4);
                std::vector<uint8_t> padding_row(new_stride - row_stride);

                write_headers(of);

                for (int y = 0; y < info_header.height; ++y)
                {
                    of.write((const char*)(data.data() + row_stride * y), row_stride);
                    of.write((const char*)padding_row.data(), padding_row.size());
                }
            }
        }
        else
        {
            throw std::runtime_error("The program can only treat 24 or 32 bits per pixel BMP files");
        }

    }
    else
    {
        throw std::runtime_error("Unable to open the output image file.");
    }
}

void BMPImage::set_pixel(float x, float y, Color color)
{
    
    if ((x < info_header.width && x >= 0) && (y < info_header.height && y >= 0))
    {
        int channels = info_header.bit_count / 8;
        data[channels * (y * info_header.width + x) + 0] = color.b;
        data[channels * (y * info_header.width + x) + 1] = color.g;
        data[channels * (y * info_header.width + x) + 2] = color.r;
        if (channels == 4)
        {
            data[channels * (y * info_header.width + x) + 3] = color.a;
        }
    }
}

void BMPImage::clear_image()
{
    for (int y = 0; y < info_header.height; y++)
    {
        for (int x = 0; x < info_header.width; x++)
        {
            int channels = info_header.bit_count / 8;
            data[channels * (y * info_header.width + x) + 0] = 0.0f;
            data[channels * (y * info_header.width + x) + 1] = 0.0f;
            data[channels * (y * info_header.width + x) + 2] = 0.0f;
            if (channels == 4)
            {
                data[channels * (y * info_header.width + x) + 3] = 255.0f;
            }
        }
    }
}

uint32_t BMPImage::make_stride_aligned(uint32_t align_stride)
{
    uint32_t new_stride = row_stride;
    while (new_stride % align_stride != 0)
    {
        new_stride++;
    }
    return new_stride;
}

void BMPImage::check_color_header(BMPColorHeader& bmp_color_header)
{
    BMPColorHeader expected_color_header;
    if (expected_color_header.red_mask != bmp_color_header.red_mask
        || expected_color_header.green_mask != bmp_color_header.green_mask
        || expected_color_header.blue_mask != bmp_color_header.blue_mask
        || expected_color_header.alpha_mask != bmp_color_header.alpha_mask)
    {
        throw std::runtime_error("Unexpected color mask format. This program expects pixel data in the BGRA format.");
    }
    if (expected_color_header.color_space_type != bmp_color_header.color_space_type)
    {
        throw std::runtime_error("Unexpected color space type! this program expects sRGB values");
    }
}

void BMPImage::write_headers(std::ofstream& of)
{
    of.write((const char*)&file_header, sizeof(file_header));
    of.write((const char*)&info_header, sizeof(info_header));
    if (info_header.bit_count == 32)
    {
        of.write((const char*)&color_header, sizeof(color_header));
    }
}

void BMPImage::write_headers_and_data(std::ofstream& of) 
{
     write_headers(of);
     of.write((const char*)data.data(), data.size());
}