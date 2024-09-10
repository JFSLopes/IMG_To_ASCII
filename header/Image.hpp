#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <stdint.h>
#include <string>
#include <fstream>
#include <istream>
#include <ostream>

struct Pixel{
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

std::istream& operator>>(std::istream& in, Pixel& p);
std::ostream& operator<<(std::ostream& os, const Pixel& p);

class Image{
private:
    Pixel** pix_map;
    uint8_t** grayscale; // One byte pixel represent how much white does a image have (0-255)
    uint16_t w;
    uint16_t h;
    std::string file;
    bool flag_error = false;

    bool read_dimensions(std::ifstream& in);
    bool read_pix_map(std::ifstream& in);
    bool allocate_pix_map();
    bool allocate_grayscale();
    void read_file();
    void make_image_grayscale_NTSC();

public:
    Image(const std::string& file);
    ~Image();

    bool loading_failed() const;
    void show_image() const;
    bool save_grayscale(const std::string& file_path) const;
};

#endif