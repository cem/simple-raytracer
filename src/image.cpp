#include <cstdlib>
#include <vector>

#include "image.h"
#include "png.h"

Image::Image(int width, int height) {
  buffer_ = new uint32_t[width * height];
  width_ = width;
  height_ = height;
}

Image::~Image() {
  delete [] buffer_;
}

uint32_t& Image::at(int x, int y) {
  return buffer_[x + y * width_];
}

void Image::set(int x, int y, unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
  uint32_t& pixel = at(x, y);
  unsigned char* components = reinterpret_cast<unsigned char*>(&pixel);
  components[0] = b;
  components[1] = g;
  components[2] = r;
  components[3] = a;
}

void Image::save(std::string filename) {
  FILE* file = fopen(filename.c_str(), "wb");
  png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  png_init_io(png_ptr, file);

  png_infop info_ptr = png_create_info_struct(png_ptr);
  png_set_IHDR(png_ptr, info_ptr, width_, height_,
               8, PNG_COLOR_TYPE_RGBA, PNG_INTERLACE_NONE,
               PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
  png_write_info(png_ptr, info_ptr);

  for (int i = 0; i < height_; ++i)
    png_write_row(png_ptr, reinterpret_cast<png_const_bytep>(buffer_ + i * width_));

  png_write_end(png_ptr, NULL);
  fclose(file);
}