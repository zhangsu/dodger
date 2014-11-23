#include <cmath>
#include <cstdio>
#include <cstring>
#include <png.h>
#include <sstream>
#include <string>
#include <zlib.h>
#include "image.hh"

Image::Image() : width_(0), height_(0), elements_(0), data_(0) {}

Image::Image(int width, int height, int elements)
    : width_(width),
      height_(height),
      elements_(elements),
      data_(new double[width_ * height_ * elements_]) {}

Image::Image(const Image& other)
    : width_(other.width_),
      height_(other.height_),
      elements_(other.elements_),
      data_(other.data_ ? new double[width_ * height_ * elements_] : 0) {
    if (data_) {
        std::memcpy(data_, other.data_,
                    width_ * height_ * elements_ * sizeof(double));
    }
}

Image::~Image() {
    delete[] data_;
}

Image& Image::operator=(const Image& other) {
    delete[] data_;

    width_ = other.width_;
    height_ = other.height_;
    elements_ = other.elements_;
    data_ = (other.data_ ? new double[width_ * height_ * elements_] : 0);

    if (data_) {
        std::memcpy(data_,
                    other.data_,
                    width_ * height_ * elements_ * sizeof(double));
    }

    return *this;
}

int Image::width() const {
    return width_;
}

int Image::height() const {
    return height_;
}

int Image::elements() const {
    return elements_;
}

double Image::operator()(int x, int y, int i) const {
    return data_[elements_ * (width_ * y + x) + i];
}

double& Image::operator()(int x, int y, int i) {
    return data_[elements_ * (width_ * y + x) + i];
}

bool Image::savePng(const std::string& filename) {
    FILE* fout = std::fopen(filename.c_str(), "wb");
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING,
                                                  nullptr, nullptr, nullptr);
    png_infop info_ptr = png_create_info_struct(png_ptr);
    setjmp(png_jmpbuf(png_ptr));

    // Setup PNG I/O.
    png_init_io(png_ptr, fout);

    // Optionally setup a callback to indicate when a row has been written.

    // Setup filtering. Use Paeth filtering.
    png_set_filter(png_ptr, 0, PNG_FILTER_PAETH);

    // Setup compression level.
    png_set_compression_level(png_ptr, Z_BEST_COMPRESSION);

    // Setup PNG header information and write it to the file.

    int color_type;
    switch (elements_) {
    case 1:
        color_type = PNG_COLOR_TYPE_GRAY;
        break;
    case 2:
        color_type = PNG_COLOR_TYPE_GRAY_ALPHA;
        break;
    case 3:
        color_type = PNG_COLOR_TYPE_RGB;
        break;
    case 4:
        color_type = PNG_COLOR_TYPE_RGBA;
        break;
    default:
        return false;
    }

    png_set_IHDR(png_ptr, info_ptr,
                 width_, height_,
                 8,
                 color_type,
                 PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_DEFAULT,
                 PNG_FILTER_TYPE_DEFAULT);
    png_write_info(png_ptr, info_ptr);

    // Actual writing.
    png_byte* tempLine = new png_byte[width_ * elements_];

    for (int i = 0; i < height_; i++) {
        for (int j = 0; j < width_; j++) {
            for (int k = 0; k < elements_; k++) {
                // Clamp the value.
                double value = std::min(1.0, std::max(0.0, (*this)(j, i, k)));

                // Write it out.
                tempLine[elements_*j+k] = static_cast<png_byte>(value * 255.0);
            }
        }
        png_write_row(png_ptr, tempLine);
    }

    delete[] tempLine;

    // closing and freeing the structs.
    png_write_end(png_ptr, info_ptr);
    png_destroy_write_struct(&png_ptr, &info_ptr);
    fclose(fout);

    return true;
}

bool Image::loadPng(const std::string& filename) {
    // check that the file is a png file.
    png_byte buf[8];

    FILE* in = std::fopen(filename.c_str(), "rb");

    if (!in)
        return false;

    for (int i = 0; i < 8; i++) {
        if (!(buf[i] = fgetc(in)))
            return false;
    }
    if (png_sig_cmp(buf, 0, 8))
        return false;

    png_structp png_ptr =
        png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
    if (!png_ptr)
        return false;

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, 0, 0);
        return false;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, 0, 0);
        return false;
    }

    png_init_io(png_ptr, in);
    png_set_sig_bytes(png_ptr, 8);
    png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, 0);

    int bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    if (bit_depth % 8) {
        png_destroy_read_struct(&png_ptr, 0, 0);
        std::ostringstream os;
        os << "Invalid bit elements " << bit_depth;
        return false;
    }

    int color_type = png_get_color_type(png_ptr, info_ptr);

    if (color_type == PNG_COLOR_TYPE_PALETTE) {
        png_set_palette_to_rgb(png_ptr);
        color_type = PNG_COLOR_TYPE_RGB;
    }

    if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8) {
        png_set_expand_gray_1_2_4_to_8(png_ptr);
    }

    if (color_type != PNG_COLOR_TYPE_RGB
        && color_type != PNG_COLOR_TYPE_GRAY
        && color_type != PNG_COLOR_TYPE_RGBA) {
        png_destroy_read_struct(&png_ptr, 0, 0);
        return false;
    }

    delete[] data_;

    width_ = png_get_image_width(png_ptr, info_ptr);
    height_ = png_get_image_height(png_ptr, info_ptr);
    switch (color_type) {
    case PNG_COLOR_TYPE_RGB:
        elements_ = 3;
        break;
    case PNG_COLOR_TYPE_RGBA:
        elements_ = 4;
        break;
    default:
        elements_ = 1;
    }

    png_bytep* row_pointers = png_get_rows(png_ptr, info_ptr);

    data_ = new double[width_ * height_ * elements_];

    for (int y = 0; y < height_; y++) {
        for (int x = 0; x < width_; x++) {
            for (int i = 0; i < elements_; i++) {
                png_byte *row = row_pointers[y];
                int index = elements_ * (y * width_ + x) + i;

                long element = 0;
                for (int j = bit_depth/8 - 1; j >= 0; j--) {
                    element <<= 8;
                    element += row[(x * elements_ + i) * bit_depth/8 + j];
                }

                data_[index] = element
                    / static_cast<double>((1 << bit_depth) - 1);
            }
        }
    }

    png_destroy_read_struct(&png_ptr, &info_ptr, 0);
    return true;
}

const double* Image::data() const {
  return data_;
}

double* Image::data() {
  return data_;
}
