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
    uint16_t w;
    uint16_t h;
    std::string file;
    bool flag_error = false;

    bool read_dimensions(std::ifstream& in);
    bool read_pix_map(std::ifstream& in);
    bool allocate_pix_map();

public:
    Image(const std::string& file);
    ~Image();

    bool loading_failed() const;
    void read_file();
    void show_image() const;
};

#endif